# -*- coding: utf-8 -*-
"""
Created on Sat Oct 10 16:24:17 2015

@author: saisn
"""

from distutils.core import setup, Extension

module1 = Extension('fimath',
include_dirs = ['/usr/local/include'],
libraries = ['python27'],
sources=['fimath.c'])

setup(name= 'fimath',
version = '1.0',
description = 'This is an example package',
author = 'saisn',
url = '',
ext_modules=[module1])