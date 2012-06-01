import Options
from os import unlink, symlink, popen
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
	opt.tool_options("compiler_cxx")

def configure(conf):
	conf.check_tool("compiler_cxx")
	conf.check_tool("node_addon")
	conf.check_cfg(package='xrandr', uselib_store='XRANDR', args='--cflags --libs')

def build(bld):
	obj = bld.new_task_gen("cxx", "shlib", "node_addon")
	obj.cxxflags = ["-Wall", "-ansi", "-pedantic"]
	obj.target = "jsdx_display"
	obj.source = """
		src/jsdx_display.cpp
		"""
	obj.cxxflags = ["-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]
	obj.uselib = "XRANDR"

def shutdown():
	if Options.commands['clean']:
		if exists('jsdx_display.node'): unlink('jsdx_display.node')
	else:
		if exists('build/default/jsdx_display.node') and not exists('jsdx_display.node'):
			symlink('build/default/jsdx_display.node', 'jsdx_display.node')
