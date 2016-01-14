import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import helper
import addline

def plotAbsTiming(allsize=1000000,allgraphtype='SOFTWARE',alladditionalwhere=' AND total_time>0 ',suffix=''):
	fig = plt.figure()
	ax = fig.add_subplot(111)
	addline.addAbsTiming(axis=ax, algorithm='locallist', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='Globallist')
	addline.addAbsTiming(axis=ax, algorithm='dynamic_nobarrier', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='DynNoBarrier')
	addline.addAbsTiming(axis=ax, algorithm='bitset', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=1,linelabel='Bitset Opt')
	addline.addAbsTiming(axis=ax, algorithm='bitset', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=4,linelabel='Bitset NoOpt')
	addline.addAbsTiming(axis=ax, algorithm='worksteal', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=2,linelabel='Worksteal Opt')
	addline.addAbsTiming(axis=ax, algorithm='worksteal', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=5,linelabel='Worksteal NoOpt')
	ax.legend(loc='upper left')
	ax.minorticks_on()

	filename = helper.plotdir + 'abstiming_gt' + allgraphtype + '_n' + str(allsize)
	
	plt.title('Time [sec] vs. Number of threads',fontsize=helper.fontsize_label)

	if(suffix==''):
		plt.suptitle('Absolute Timing for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes)',fontsize=helper.fontsize_title)
	else:
		plt.suptitle('Absolute Timing for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes, ' + suffix + ')',fontsize=helper.fontsize_title)
		filename = filename + '_' + suffix

	filename = filename + '.pdf'
	
	plt.savefig(filename, format='pdf',bbox_inches='tight',dpi=1000)
	print "File written to:\t", filename
	if(helper.show):
		plt.show()


############################################################
# Call Plotting functions
############################################################
plotAbsTiming(allsize=1000000,allgraphtype='SOFTWARE') # software graph
plotAbsTiming(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=7999910',suffix='deg8') # degree 8
plotAbsTiming(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=15999722',suffix='deg16') # degree 16
plotAbsTiming(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=31998947',suffix='deg32') # degree 32
plotAbsTiming(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=63995794',suffix='deg64') # degree 64
