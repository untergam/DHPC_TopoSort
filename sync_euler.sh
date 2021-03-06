#!/bin/bash

MYABSREPOPATH=/home/matthias/polybox/CSE7/DesignHPC/project/DHPC_TopoSort
CLUSTERNAME=euler.ethz.ch
CLUSTERSRCPATH=dphpc_toposort;
MYETHUSERNAME=untergam


# copies from current machine to cluster and vice versa
#		-u	keep local files if newer than other file
#		-z	compress files

# You must have a ssh key registered on the cluster, check
#		http://wacc.wisc.edu/docs/accessing/ --> Key-Based Authentication
# 
# No warranty here, but it works great for me
# Use your own skript instead of this if you want

# send source code to cluster
echo -e "Sending to cluster...\n"
rsync -aiuz ${MYABSREPOPATH}/src/ ${MYETHUSERNAME}@${CLUSTERNAME}:${CLUSTERSRCPATH}/ --exclude '*.o' --exclude '*.exe' --exclude '*.xml' --exclude 'results/' --exclude 'graph_output/' --exclude 'dryrun/'

# receive from cluster (include not strictly necessary, but more explicit)
echo -e "Receiving from cluster...\n"
rsync -aiuz ${MYETHUSERNAME}@${CLUSTERNAME}:${CLUSTERSRCPATH}/ ${MYABSREPOPATH}/src/ --exclude '*.o' --exclude '*.exe' --include '*.xml' --include 'results/' --include 'graph_output/' --exclude 'dryrun/'

exit 0
