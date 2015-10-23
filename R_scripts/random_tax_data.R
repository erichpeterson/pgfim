# change to the new directory
setwd("/Users/eapeterson/MegaCloud/Research/Code/PGFIM/PFGIM/R_scripts")
# load the necessary libraries
library(igraph)

DAG.random <- function(v, nedges=1) {
    edges.max <- v*(v-1)/2
    # Assert length(v)==1 && 1 <= v
    # Assert 0 <= nedges <= edges.max
    index.edges <- lapply(list(1:(v-1)), function(k) rep(k*(k+1)/2, v-k)) 
    index.edges <- index.edges[[1]] + 1:edges.max
    graph.adjacency <- matrix(0, ncol=v, nrow=v)
    graph.adjacency[sample(index.edges, nedges)] <- 1
    graph.adjacency
}

PGFIM.random.graph <- function(v, leafs, nedges=1) {
	adj_matrix <- DAG.random(v, nedges)
	#print(adj_matrix)
	g <- graph.adjacency(adj_matrix, mode="directed", diag=FALSE)
	connected_comp <- clusters(g, mode="weak")
	#print(connected_comp$membership)
	
	g <- add.vertices(g, 1)
	index_for_root <- v
	v <- v + 1
	
	if (connected_comp$no != 1) {
		#print("1")
		i <- 0
		while (i < connected_comp$no) {
			j <- 1
			while (j < v) {
				if (connected_comp$membership[j] == i) {
					num_in <- subcomponent(g, j - 1, mode="in")
				
		 			if (length(num_in) == 1) {
						g <- add.edges(g, c(index_for_root, j - 1))
					}
				}
				j <- j + 1
			}
			i <- i + 1	
		}
	}
	else {
		#print("2")
		flag <- 0
		i <- v - 1
		j <- 1
		while (j <= i) {
			num_in <- subcomponent(g, j - 1, mode="in")
			
			if (length(num_in) == 1) {	
				g <- add.edges(g, c(index_for_root, j - 1))
			}
			j <- j + 1
		}
	}
	
	g_leafs <- numeric(0)

	j <- 1
	while (j <= v) {
		num_out <- subcomponent(g, j - 1, mode="out")
		if (length(num_out) == 1) {
			g_leafs <- c(g_leafs, j - 1)
		}
		j <- j + 1
	}

	num_g_leafs <- length(g_leafs)
	
	#print(g_leafs)
	#print(num_g_leafs)
	
	if (num_g_leafs > leafs) {
		stop("The number of leafs to be added is less than the number of possible parents.")
	}
	
	i <- 0
	while (i < num_g_leafs) {
		lower_bound <- floor((i * leafs) / num_g_leafs)
		upper_bound <- floor(((i + 1) * leafs) / num_g_leafs) - 1
		#print(lower_bound)
		#print(upper_bound)
		
		j <- lower_bound
		while(j <= upper_bound) {
			g <- add.vertices(g, 1)
			v <- v + 1
			g <- add.edges(g, c(g_leafs[i + 1], v - 1))
			j <- j + 1
		}
		i <- i + 1
	}
	
	i <- v
	
	while (i > 0) {
		V(g)[i - 1]$name = v - i + 1
		i <- i - 1
	}
	
	# Randomly rename the leaf nodes
	random_leaf_names <- sample(1:leafs)
	
	#print(random_leaf_names)
	
	i <- v - leafs
	while (i < v) {
		V(g)[i]$name = random_leaf_names[v - i]
		i <- i + 1
	}
	
	V(g)[index_for_root]$name = -1
	
	V(g)$label <- V(g)$name
	
	# Sanity check: Check to make sure there are no cycles and
	# that there are only 'leafs' number of leaf nodes in the graph
	
	if(!is.simple(g)) {
		stop("The graph is not simple.")
	}
	
	final_graph_leafs <- numeric(0)

	j <- 1
	while (j <= v) {
		num_out <- subcomponent(g, j - 1, mode="out")
		if (length(num_out) == 1) {
			final_graph_leafs <- c(final_graph_leafs, j - 1)
		}
		j <- j + 1
	}

	num_final_leafs <- length(final_graph_leafs)
	
	#print(num_final_leafs)
	
	if(num_final_leafs != leafs) {
		stop("The graph does not have the correct number of leaf nodes.")
	}
	
	dest_filename <- paste0("V", index_for_root, "E", nedges, ".xml")
 	
 	dest <- file(dest_filename, "w")
 	
 	writeLines("<?xml version=\"1.0\" standalone=no>", dest)
 	writeLines("<dag>", dest)
 	writeLines("    <vertices>", dest)
 	
 	i <- 0
 	
 	while (i < vcount(g)) {
 		line <- paste0("        <vertex value=\"", V(g)[i]$name, "\" />")
 		writeLines(line, dest)
 		i <- i + 1
 	}
 	writeLines("    </vertices>", dest)
 	
 	writeLines("    <edges>", dest)
 	
 	i <- 1
 	edge_list <- get.edgelist(g)
 	while (i <= ecount(g)) {
 		line <- paste0("        <edge from=\"", edge_list[i,1], "\"", " to=\"", edge_list[i,2], "\" />")
 		writeLines(line, dest)
 		i <- i + 1
 	}
 	
 	writeLines("    </edges>", dest)
 	writeLines("</dag>", dest)
 	
 	close(dest)
	
	layout.fruchterman.reingold.grid(g)
	plot.igraph(g)
}