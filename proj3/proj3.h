/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 */

/**
 * @brief This is a structure, which represents certain object with unique coordinations.
 */
struct obj_t {
    int id;
    float x;
    float y;
};
/**
 * @brief This is a structure, which represents clusters with objects.
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @defgroup clusters Cluster processing
 * @{
 */

/**
 * Initialization of cluster 'c'. 
 * Allocating the memory for the 'cap' of the object (capacity). 
 * Pointer NULL in object array means that capacity is 0.
 *
 * @param c   cluster for initialization
 * @param cap capacity of cluster
 */
void init_cluster(struct cluster_t *c, int cap);
/**
 * Remove all objects from cluster and initialise the empty cluster.
 *
 * @post
 * Allocated memory of cluster 'c' will be freed.
 * 
 * @param c cluster which is going to be removed
 */
void clear_cluster(struct cluster_t *c);
/**
 * This is a value for memory reallocation
 */
extern const int CLUSTER_CHUNK;
/**
 * Capacity of cluster 'c'  will be changed to 'new_cap'.
 *
 * @pre 
 * Capacity of cluster 'c' will be greater or equal to 0.
 *
 * @post 
 * Capacity of cluster 'c' will be changed to the 'new_cap'.
 * 
 * @param  c       cluster for capacity change
 * @param  new_cap new capacity of cluster
 * @return         cluster with a new capacity, in case of ERROR returns NULL
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);
/**
 * Adds an object 'obj' to the end of the cluster 'c'. 
 * If there is not enough space for an object, cluster will be extended.
 *
 * @pre
 * Number of objects in cluster will be greater or equal to 0.
 *
 * @post
 * Object 'obj' will be added to the end of cluster 'c', if there will be no ERROR
 * 
 * @param c   cluster for added object
 * @param obj object which will be added to cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);
/**
 * Adds objects into the cluster 'c1' from cluster 'c2'. 
 * Cluster 'c1' will be extended if needed. 
 * Objects in cluster 'c1' will be lined up ascending. 
 * Cluster 'c2' remains unchanged.
 *
 * @pre
 * Cluster 'c2' won't be empty.
 *
 * @post
 * Cluster 'c1' will be extened by number of added objects from cluster 'c2'
 *
 * @post
 * Objects in 'c1' will be lined up ascending by ID.
 * 
 * @param c1 cluster which will be filled with objects from cluster 'c2'
 * @param c2 objects from this cluster will be added into the cluster 'c1'
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);
/**
 * @}
 */

/**
 * @defgroup
 */

/**
 * Removes cluster from array of clusters 'carr'. 
 * Array of clusters contains 'narr' items (clusters). 
 * Cluster for removing is placed on index 'idx'. 
 * Function returns new count of clusters in array.
 *
 * 
 * @param  carr array of clusters
 * @param  narr number of clusters in array
 * @param  idx  index of cluster, which will be removed
 * @return      new count of clusters in array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);
/**
 * Counts Euclidean distance between two objects.
 * 
 * @param  o1 object 1
 * @param  o2 object 2
 * @return    Euclidean distance between objects 'o1' and 'o2'
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);
/**
 * Counts distance of two clusters.
 *
 * @pre
 * Number of objects in cluster 'c1' will be greater than 0.
 *
 * @pre
 * Number of objects in cluster 'c1' will be greater than 0.
 * 
 * @param  c1 cluster 1
 * @param  c2 cluster 2
 * @return    distance of cluster 'c1' and 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);
/**
 * Function finds two nearest clusters.
 *
 * @param carr array of clusters
 * @param narr number of clusters in array
 * @param c1   index of the first found cluster
 * @param c2   index of the second found cluster
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @addtogroup clusters
 * @{
 */

/**
 * Ascending lining up of objects in cluster by their ID.
 * 
 * @post
 * Objects in cluster 'c' will be lined up ascending by ID.
 * 
 * @param c cluster for line up
 */
void sort_cluster(struct cluster_t *c);
/**
 * Print of cluster 'c' to stdout.
 *
 * @post
 * Objects of cluster 'c' will be printed to stdout
 * 
 * @param c cluster which will be printed
 */
void print_cluster(struct cluster_t *c);
/**
 * @}
 */

/**
 * Loads objects from file 'filename'. 
 * Creates a cluster for each object and stores it in the array of cluster. 
 * Allocates the memory space for the array of clusters and a pointer onto
 * the first item in array (pointer onto the first cluster in allocated array)
 * stores into memory, where parameter 'arr' is reffering.
 * The function returns the number of loaded objects (clusters).
 * In case of some kind of error, stores in the memory, 
 * where 'arr' is referring, value NULL.
 *
 * @pre 
 * Programme will have rights to read an existing file 'filname'
 *
 * @post
 * For each object in file 'filename' will be created a cluster,
 * all of these clusters will be stored in array of clusters 'arr',
 * which will have allocated memory for number of clusters that is
 * indicated in file 'filename', in case there is not error.
 *
 * @param  filename name of the file which will be readed
 * @param  arr      pointer onto an array of clusters loaded from file (points onto NULL if there is an error)
 * @return          number of loaded objects (-1 if there is an error)
 */
int load_clusters(char *filename, struct cluster_t **arr);
/**
 * Prints array of clusters
 *
 * @post
 * Objects of all clusters in array 'carr' will be printed to stdout
 * 
 * @param carr array of clusters which will be printed
 * @param narr number of clusters in array
 */
void print_clusters(struct cluster_t *carr, int narr);