#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include <mach-o/dyld.h>

using namespace std;

void replace_all(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;

    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

int main(int argc, char** argv)
{
    char* program_copy = strdup(argv[0]);
    char* program_name = basename(program_copy);
    free(program_copy);

    if (strcmp(program_name, "wrapper.bin") == 0) {
        printf("usage: symlink %s to an application of your choice\n", program_name);
        return 64;
    }

    char binary_path[2048];
    uint32_t size = sizeof(binary_path);
    if (_NSGetExecutablePath(binary_path, &size) != 0) {
        fprintf(stderr, "failed to get executable path!");
        return 1;
    }

    char* binary_dirname = dirname(binary_path);

    char* absolute_path = realpath(binary_dirname, NULL);
    string absolute_path_string(absolute_path);
    free(absolute_path);

    string path(getenv("PATH"));
    absolute_path_string.append(":");
    replace_all(path, absolute_path_string, "");
    setenv("PATH", path.c_str(), 1);

    char* working_path = realpath(".", NULL);
    printf("!!!xmake!!! PWD=%s %s", working_path, program_name);
    free(working_path);

    for (int i = 1; i < argc; i++) {
        // FIXME: We might need better detection of what we concider the
        // input file than just looking at the extension.
        if (strcmp(argv[i] + strlen(argv[i]) - 4, ".cpp") == 0
            || strcmp(argv[i] + strlen(argv[i]) - 2, ".c") == 0
            || strcmp(argv[i] + strlen(argv[i]) - 3, ".mm") == 0
            || strcmp(argv[i] + strlen(argv[i]) - 2, ".m") == 0) {

            // By making the input filename absolute, we trick clang into
            // outputting errors and warnings with an absolute path as well,
            // which lets Xcode correctly pinpoint the file in question.
            argv[i] = realpath(argv[i], NULL);
        }

        printf(" %s", argv[i]);
    }
    printf("\n");

    fflush(stdout);
    execvp(program_name, argv);

    fprintf(stderr, "xmake: command not found: %s\n", program_name);
    return 127;
}