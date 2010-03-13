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
	autowaf.check_pkg(conf, 'aubio', uselib_store='AUBIO', atleast_version='0.3.3', mandatory=True)
	autowaf.check_pkg(conf, 'sndfile', uselib_store='SNDFILE', atleast_version='1.0.17', mandatory=True)
	
	# Boost headers
	autowaf.check_header(conf, 'boost/ptr_container/ptr_container.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/thread.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/shared_ptr.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/random.hpp', mandatory=True)
	
	conf.env.append_value('LINKFLAGS', '-lboost_thread-mt')

def build(bld):

	reverbtuner = bld.new_task_gen('cxx', 'program')
	reverbtuner.source = '''
		src/main.cc
		src/data_source.cc
		src/evolutionary_optimizer.cc
		src/evaluation_set.cc
		src/lv2_plugin.cc
		src/lv2_plugin_factory.cc
		src/lv2_world.cc
		src/mfcc_evaluator.cc
		src/mfcc_processor.cc
		src/parameter_modifier.cc
		src/parameter_values.cc
	'''
	reverbtuner.includes     = '.'
	reverbtuner.uselib       = 'SLV2 AUBIO SNDFILE'
	reverbtuner.target       = 'reverbtune'
	reverbtuner.install_path = ''

def shutdown():
	autowaf.shutdown()

