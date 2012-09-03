# Trick mkmf into linking a binary instead of a shared library
require 'rbconfig'
RbConfig::MAKEFILE_CONFIG['LINK_SO'] = "$(CXX) -o $@ $(OBJS) $(LDFLAGS)"

require 'mkmf'
CONFIG['DLEXT'] = "bin"
create_makefile('../bin/wrappers/wrapper') do |config|
	config = configuration('') << <<RULES
all:
install: symlink
symlink: install-so
        ln -s $(RUBYARCHDIR)/$(DLLIB) $(RUBYARCHDIR)/clang
        ln -s $(RUBYARCHDIR)/$(DLLIB) $(RUBYARCHDIR)/clang++
RULES
end