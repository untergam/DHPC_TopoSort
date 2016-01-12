import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import helper


def addStrongScaling(axis, algorithm, optimistic, size, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='nolabel'):

	fixedwhere = "enable_analysis=0 AND debug=0 AND verbose=0 AND processors>=number_of_threads AND algorithm='{0}' AND optimistic={1} AND graph_type='{2}' AND hostname LIKE '{3}' AND graph_num_nodes={4}".format(algorithm,optimistic,graphtype,hostnamelike,size)

	numthreads = helper.getData('number_of_threads', fixedwhere + ' GROUP BY number_of_threads')

	avgtimings = []

	if (np.size(numthreads)==0):
		return

	for nt in numthreads.flat:
		# print "NUMTHREADS = ", nt
		where = fixedwhere + ' AND number_of_threads={0}'.format(nt)
		timings = helper.getData('total_time',where)
		avgtimings.append(np.mean(timings))
		speedups = avgtimings[0]/timings

		violin_parts = ax.violinplot(speedups,[nt],widths=0.8)

		for pc in violin_parts['bodies']:
			pc.set_color(helper.getFGcolor(colorindex))

	speedup = avgtimings[0]/avgtimings

	ax.plot(numthreads,speedup,'D-',markersize=4,linewidth=1,color=helper.getFGcolor(colorindex),label=linelabel) # connecting dots







############################################################
# Set up Plot and add scaling lines 
############################################################

##########################
# Software Graph
fig = plt.figure()
ax = fig.add_subplot(111)


addStrongScaling(axis=ax, algorithm='locallist', optimistic='0', size=1000000, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='Globallist')

addStrongScaling(axis=ax, algorithm='bitset', optimistic='1', size=1000000, graphtype='SOFTWARE', hostnamelike='e%',colorindex=1,linelabel='Bitset Opt')

addStrongScaling(axis=ax, algorithm='bitset', optimistic='0', size=1000000, graphtype='SOFTWARE', hostnamelike='e%',colorindex=4,linelabel='Bitset NoOpt')

addStrongScaling(axis=ax, algorithm='worksteal', optimistic='1', size=1000000, graphtype='SOFTWARE', hostnamelike='e%',colorindex=2,linelabel='Worksteal Opt')

addStrongScaling(axis=ax, algorithm='worksteal', optimistic='0', size=1000000, graphtype='SOFTWARE', hostnamelike='e%',colorindex=5,linelabel='Worksteal NoOpt')

ax.plot(range(1,24),range(1,24),'r--') # ideal scaling


# handles, labels = ax.get_legend_handles_labels()
# ax.legend(handles,labels)
ax.legend()

plt.title('Strong Scaling',fontsize=helper.fontsize_title)
plt.xlabel('Number of threads',fontsize=helper.fontsize_label)
plt.ylabel('Speedup',fontsize=helper.fontsize_label)

ax.minorticks_on()

plt.savefig(helper.plotdir + 'strongscaling_gtSOFTWARE.pdf',format='pdf',bbox_inches='tight',dpi=1000)
plt.show()

