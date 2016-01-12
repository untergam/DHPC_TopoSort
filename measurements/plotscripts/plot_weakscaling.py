import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import colortableau as ct

# plt.style.use('ggplot')
plotdir = "plots/";
db = sqlite3.connect('measurements.db')

query = db.cursor()



def getData(field, wherestring):
	with db:
		querystring = "SELECT {0} FROM measurements WHERE {1}".format(field,wherestring)
		query.execute(querystring)
		print(querystring)
		data = query.fetchall()
	
	return np.array(data)


def addWeakScaling(axis, algorithm, optimistic, size, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='nolabel'):

	fixedwhere = "enable_analysis=0 AND debug=0 AND verbose=0 AND processors>=number_of_threads AND algorithm='{0}' AND optimistic={1} AND graph_type='{2}' AND hostname LIKE '{3}' AND graph_num_nodes={4}*number_of_threads".format(algorithm,optimistic,graphtype,hostnamelike,size)

	numthreads = getData('number_of_threads', fixedwhere + ' GROUP BY number_of_threads')

	print(numthreads)

	avgtimings = []

	if (np.size(numthreads)==0):
		return

	for nt in numthreads.flat:
		# print "NUMTHREADS = ", nt
		where = fixedwhere + ' AND number_of_threads={0}'.format(nt)
		timings = getData('total_time',where)
		avgtimings.append(np.mean(timings))
		speedups = avgtimings[0]/timings

		violin_parts = ax.violinplot(speedups,[nt],widths=0.8)

		for pc in violin_parts['bodies']:
			pc.set_color(ct.getFGcolor(colorindex))

	speedup = avgtimings[0]/avgtimings

	ax.plot(numthreads,speedup,'D-',markersize=4,linewidth=1,color=ct.getFGcolor(colorindex),label=linelabel) # connecting dots







############################################################
# Set up Plot and add scaling lines 
############################################################

##########################
# Software Graph
fig = plt.figure()
ax = fig.add_subplot(111)

basesize=100000

addWeakScaling(axis=ax, algorithm='locallist', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='Globallist')
addWeakScaling(axis=ax, algorithm='bitset', optimistic='1', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=1,linelabel='Bitset Opt')
addWeakScaling(axis=ax, algorithm='bitset', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=4,linelabel='Bitset NoOpt')
addWeakScaling(axis=ax, algorithm='worksteal', optimistic='1', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=2,linelabel='Worksteal Opt')
addWeakScaling(axis=ax, algorithm='worksteal', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=5,linelabel='Worksteal NoOpt')

ax.plot([1,24],[1,1],'r--')

# handles, labels = ax.get_legend_handles_labels()
# ax.legend(handles,labels)
ax.legend()

plt.title('Weak Scaling',fontsize=ct.fontsize_title)
plt.xlabel('Number of threads',fontsize=ct.fontsize_label)
plt.ylabel('Speedup',fontsize=ct.fontsize_label)

ax.minorticks_on()

plt.savefig(plotdir + 'weakscaling_gtSOFTWARE.pdf',format='pdf',bbox_inches='tight',dpi=1000)
plt.show()



db.close();
