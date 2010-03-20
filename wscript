#!/usr/bin/env python
# -*- coding: utf-8 -*-
import autowaf
import Options

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
	autowaf.check_pkg(conf, 'gtkmm-2.4', uselib_store='GTKMM', atleast_version='2.8', mandatory=True)
	
	# Boost headers
	autowaf.check_header(conf, 'boost/ptr_container/ptr_container.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/thread.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/shared_ptr.hpp', mandatory=True)
	autowaf.check_header(conf, 'boost/random.hpp', mandatory=True)
	
	conf.env.append_value('LINKFLAGS', '-lboost_thread-mt')
	
	optimization_flags = [
		"-O3",
		"-fomit-frame-pointer",
		"-ffast-math",
		"-fstrength-reduce",
		"-pipe"
		]

	if not Options.options.debug:
		conf.env.append_value('CCFLAGS', optimization_flags)
		conf.env.append_value('CXXFLAGS', optimization_flags)
		conf.env.append_value('LINKFLAGS', optimization_flags)

def build(bld):

	reverbtuner = bld.new_task_gen('cxx', 'cshlib')
	reverbtuner.source = '''
		src/basic_evaluation_set.cc
		src/data_source.cc
		src/evolutionary_optimizer.cc
		src/lv2_plugin.cc
		src/lv2_plugin_factory.cc
		src/lv2_world.cc
		src/mfcc_evaluator.cc
		src/mfcc_processor.cc
		src/parameter_modifier.cc
		src/parameter_values.cc
		src/runner.cc
	'''
	reverbtuner.includes     = '.'
	reverbtuner.uselib       = 'SLV2 AUBIO SNDFILE'
	reverbtuner.target       = 'reverbtuner'
	reverbtuner.install_path = ''
	reverbtuner.name         = 'reverbtuner'
	
	gui = bld.new_task_gen('cxx', 'cprogram')
	gui.source = '''
		gui/main.cc
		gui/file_selection_view.cc
		gui/main_assistant.cc
		gui/parameter_view.cc
		gui/plugin_selection_view.cc
		gui/progress_plot.cc
		gui/progress_view.cc
		gui/waveform.cc
	'''
	gui.includes     = '.'
	gui.uselib       = 'GTKMM'
	gui.uselib_local = 'reverbtuner'
	gui.target       = 'reverbtuner-gui'
	gui.install_path = ''
	

def shutdown():
	autowaf.shutdown()

