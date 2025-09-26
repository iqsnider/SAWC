project('SAWC', 'cpp',
version : '0.1.0',
default_options : ['cpp_std=c++17'])
sdl2_dep = dependency('sdl2', required : false)

if not sdl2_dep.found()
  homebrew_prefix = '/opt/homebrew'
  sdl2_dep = declare_dependency(
    include_directories: include_directories(homebrew_prefix / 'include'),
    link_args: ['-L' + homebrew_prefix / 'lib', '-lSDL2']
  )
endif

executable('main', 'main.cpp', dependencies : sdl2_dep)
