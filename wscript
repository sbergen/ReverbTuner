#!/usr/bin/env python
# -*- coding: utf-8 -*-
import autowaf

# Version of this package (even if built as a child)
REVERB_TUNER_VERSION = '0.0.0'

# Variables for 'waf dist'
APPNAME = 'reverbtuner'
VERSION = REVERB_TUNER_VERSION

# Mandatory variables
srcdir = '.'
blddir = 'build'

def set_options(opt):
	autowaf.set_options(opt)

def configure(conf):
	autowaf.configure(conf)
	
	conf.check_tool('compiler_cxx')
	
	autowaf.check_pkg(conf, 'slv2', uselib_store='SLV2', atleast_version='0.6.6', mandatory=True)
	
	# Boost headers
	autowaf.check_header(conf, 'boost/ptr_container/ptr_map.hpp')

def build(bld):

	reverbtuner = bld.new_task_gen('cxx', 'program')
	reverbtuner.source = '''
		src/main.cc
		src/parameter.cc
		src/parameter_values.cc
		src/lv2_plugin.cc
		src/lv2_plugin_factory.cc
		src/lv2_world.cc
	'''

	reverbtuner.uselib       = 'SLV2'
	reverbtuner.target       = 'reverbtuner'
	reverbtuner.install_path = ''

def shutdown():
	autowaf.shutdown()

