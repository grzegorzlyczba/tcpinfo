import os

srcdir ='.'
bldir = 'build'
version = '1.0'

def set_options(opt):
	opt.tool_options('compiler_cxx');

def configure(conf):
	conf.check_tool('compiler_cxx');
	conf.check_tool('node_addon');

def build(bld):
	obj = bld.new_task_gen('cxx', 'shlib', 'node_addon');
	obj.target = 'tcpinfo'
	obj.source = 'src/node-tcpinfo.cc'

def shutdown():
	if os.path.exists("build/default/tcpinfo.node"):
		os.system("cp build/default/tcpinfo.node ./bin");
