# -*- coding: utf-8 -*-
"""
Created on Thu Jul  6 13:41:08 2017

@author: tim
"""

import os
from os import path
from sys import stdout, stderr
    
def crawl_dir(directory = os.getcwd(), deepest_first = True):
    ''' 
        Navigate a directory in a DFS pattern returning the full path to each
        file in the root directory and in each subdirectory.  
        
        If there exists a '.ignore' file in any of the (sub)directories parsed, 
        crawl_dir() will not return any files listed in '.ignore'. (each file 
        should be listed on its own line in '.ignore').  '.ignore' is itself 
        implicitly ignored.
        
        Keyword Args:
            directory -     The directory to crawl through.  Defaults to the 
                            current working directory (the directory of the 
                            calling process.)
            deepest_first - Return files in the deepest directories first.
            
        Return:
            A generator that generates string pathnames.
    '''
    for item in os.walk(directory, not deepest_first):
        path, dirs, files = item
        if ".ignore" in files:
            with open(path + "/.ignore", "rt") as ignores:
                for line in ignores:
                    if line in files:
                        files.remove(line)
                    elif line.strip() in files:
                        files.remove(line.strip())
            files.remove(".ignore")
        for fname in files:
            yield path + "/" + fname



 

                