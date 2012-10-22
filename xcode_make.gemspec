Gem::Specification.new do |s|
  s.name        = 'xcode_make'
  s.version     = '0.0.2'
  s.date        = '2012-09-04'
  s.summary     = "Xcode-make!"
  s.description = "A wrapper for using Xcode's indexer with external targets"
  s.authors     = ["Tor Arne Vestbø"]
  s.email       = 'torarnv@gmail.com'
  s.files       = Dir.glob('ext/**/*.{cpp,rb}')
  s.executables = ['xcode-make']
  s.homepage    = 'https://github.com/torarnv/xcode-make'

  s.required_ruby_version = Gem::Requirement.new(">= 1.9.2")
  s.add_runtime_dependency("xcoder", ">= 0.1.13")

  s.extensions << 'ext/xcode_make/extconf.rb'
end
