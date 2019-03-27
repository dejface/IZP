/**
 * 			Project 3 - Simple cluster analysis
 * Author: David Oravec - xorave05
 *
 * This programme merge and sort objects into clusters by using the single linkage
 * cluster analysis.
 * 
 * The single linkage method selects two clusters that are closest to each other 
 * The distance of two clusters is equal to the smallest distance of any of two 
 * objects from both clusters.
 *
 * Usage: ./proj3 FILENAME N
 * FILENAME = file with objects
 * N = number of clusters
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */
/*
This is a structure, which represents certain object
with unique coordinations.
 */
struct obj_t {
    int id;	
    float x;
    float y;
};
/*
This is a structure, which represents the cluster of objects
 */
struct cluster_t {
    int size;			//number of objects in cluster
    int capacity;		//capacity of cluster
    struct obj_t *obj;  //array of objects
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
Counting maximum number od digits in 'n' number
 */
int max_int_number(int n){
	int int_count_after_fraction = 0;
	while (abs(n) != 0){
		n /= 10;
		int_count_after_fraction++;
	}
	return int_count_after_fraction;
}

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    if (cap > 0){
    	//memmory allocation for object array
    	if ((c->obj = malloc(cap * sizeof(struct obj_t)))){
    		c->capacity = cap;
    		return;
    	}
    }
    //if the allocation goes wrong, or capacity is 0
    c->capacity = 0;
    c->obj = NULL;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    init_cluster(c,0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    assert(c);
    assert(c->capacity <= c->size);
    if (c->capacity <= c->size){
    	//capacity extension
    	if (! resize_cluster(c,c->capacity + CLUSTER_CHUNK)){
    		return;
    	}
    }
    //adding an object on the first possible free position,extending size of cluster
    c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    int old_size = c1->size;
    for (int i = 0; i < c2->size; i++){
    	append_cluster(c1,c2->obj[i]);
    }
    if (c1->size == c2->size + old_size && c2->size > 0){
    	sort_cluster(c1);
    }
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
    // deallocation of memmory for cluster with index idx
    clear_cluster(&carr[idx]);
    //new size of narr will be decreased by one
    int new_narr = narr - 1;
    for (int i = idx; i < new_narr;i++){
    	carr[i] = carr[i+1];
    }
    
    return new_narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float distance = 0.0, x_count = 0.0, y_count = 0.0;
    x_count = powf(o1->x - o2->x,2.0);
    y_count = powf(o1->y - o2->y,2.0);
    distance = sqrtf(x_count + y_count);

    return distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    //clust_distance is set to 2 000 000,because maximum nubmer of y and x
    //can be 1000, which means 1000^2 + 1000^2
    float distance,clust_distance = sqrtf(2000000);
    // counting the distance between objects in c1 and c2 clusters
    for (int i = 0; i < c1->size;i++){
    	for (int j = i; j < c2->size; j++){
    		distance = obj_distance(&c1->obj[i], &c2->obj[j]);
    		if (distance < clust_distance){
    			clust_distance = distance;
    		}
    	}
    }
    return clust_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    //if there is only one cluster in the array
    if (narr == 1){
    	*c1 = 0;
    	*c2 = 0;
    	return;
    }

    float min_distance = -42, distance;
    //counting the distance among every cluster
    for (int i = 0; i < narr; i++){
    	for (int j = i + 1; j < narr; j++){
    		distance = cluster_distance(&carr[i],&carr[j]);
    		// veryfing minimal distance, if it is equal to -42, means basic/
    		// unchanged value
    		if (min_distance == -42 || distance < min_distance){
    			min_distance = distance;
    			*c1 = i;
    			*c2 = j;
    		}
    	}
    }    
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}
/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}
/*
function which creates an array of clusters and allocates
memmory for 'narr' clusters
 */ 
void init_clusters(struct cluster_t **carr, int narr){
	assert (carr);
	assert (narr >= 0);
	// allocation of memmory for an array of clusters
	if (! (*carr = malloc(narr * sizeof(struct cluster_t)))){
		return;
	}
	//initialization of an array object
	for (int i = 0; i < narr; i++){
		init_cluster(&(*carr)[i],0);
	}
}
/*
function which removes cluster array
 */
void clear_clusters(struct cluster_t *carr, int narr) {
	//deallocation of array object
	for (int i = 0; i < narr; i++){
		clear_cluster(&carr[i]);
	}
	free(carr);
}


/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    FILE *file = fopen(filename,"r");

    if (!file){
    	fprintf(stderr,"File couldn't be opened.\n");
    	return -1;
    }

    int max_line_length = max_int_number(INT_MAX) + 12;
    char line[max_line_length];

    int obj_id, obj_x, obj_y, load_objs = 0, number_of_line = 0;
    struct obj_t obj;
    struct cluster_t *cluster;
    char term_char;

    while (fgets(line,max_line_length,file) != NULL) {
    	++number_of_line;
    	if (number_of_line == 1) {
    		//after the number of objects, there cannot be any character except
    		// '\0' and '\n'
    		if (sscanf(line, "count=%d%[^\n]",&load_objs,&term_char) != 1) {
    			fprintf(stderr,"Bad format in the .txt file on %d. line.\n",
    				    number_of_line);
    			fclose(file);
    			return -1;
    		}

    		if (load_objs <= 0){
    			fprintf(stderr,"The number of clusters should be greater than 0"
    			                "you have entered %d.\n",load_objs);
    			fclose(file);
    			return -1;
    		}
    		//cluster array initialization
    		init_clusters(arr, load_objs);
    		if (*arr == NULL) {
    			fprintf(stderr,"ERROR: Memmory couldn't be allocated\n");
    			fclose(file);
    			return -1;
    		}
    		continue;
    	}
    	if (number_of_line - 1 > load_objs){
    		break;
    	}

    	if (sscanf(line,"%d %d %d %[^\n]",&obj_id, &obj_x, &obj_y, &term_char) != 3
    		|| obj_x > 1000 || obj_x < 0 || obj_y > 1000 || obj_y < 0){
    		fprintf(stderr,"Bad format in the .txt file on %d. line\n",
    			    number_of_line);
    		clear_clusters(*arr,load_objs);
   			*arr = NULL;
   			fclose(file);
   			return -1;
   		}
    		
   		obj.id = obj_id;
   		obj.x = obj_x;
   		obj.y = obj_y;

   		//adding an object to the particular cluster
   		//CAREFUL! objects are located on the second line, however index is
   		//counted from 0 (because of that -> -2) 
    	cluster = &(*arr)[number_of_line - 2];
    	append_cluster(cluster,obj);
   		if (cluster->size != 1){
   			fprintf(stderr,"ERROR: Memmory couldn't be allocated\n");
   			clear_clusters(*arr, load_objs);
   			*arr = NULL;
   			fclose(file);
   			return -1;
   		}
   	}
   	fclose(file);
   	//number of objects cannot
   	if (number_of_line - 1 < load_objs){
   		fprintf(stderr,"Number of objects in file ( %d ) have to be equal to\n"
   			           "the value of 'count=%d' !\n", number_of_line,load_objs);
   		return -1;
   	}

return load_objs;
}

int final_cluster(struct cluster_t *clusters, int size, int needed_size){
	int c1_idx, c2_idx, previous_c1_size;
		while (size > needed_size){
			find_neighbours(clusters, size, &c1_idx, &c2_idx);
			previous_c1_size = clusters[c1_idx].size;
			merge_clusters(&clusters[c1_idx], &clusters[c2_idx]);
			if (clusters[c2_idx].size > 0 && clusters[c1_idx].size != previous_c1_size + clusters[c2_idx].size){
				fprintf(stderr,"ERROR: Memmory couldn't be allocated.");
				return -1;
			}
			size = remove_cluster(clusters, size, c2_idx);
		}
	return size;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
	if (argc > 1 && argc <= 3){
		struct cluster_t *clusters;
		int size, needed_size = 1; 

		if (argc == 3) {
			char *endptr;
			needed_size = (int) strtol(argv[2],&endptr,10);

			if (*endptr){
				fprintf(stderr,"Value on %d. argument has to be an integer number.\n"
				                "Given %s.\n",argc, endptr);
				return 1;
			} else if (needed_size <= 0){
				fprintf(stderr,"Value on %d. argument has to be greater than 0\n"
				                "Given %d.\n",argc, needed_size);
				return 1;
			}
		}
		if ((size = load_clusters(argv[1],&clusters)) == -1){
			return 1;
		}
		if (needed_size > size){
			fprintf(stderr,"The number of clusters that you have entered ( %d ),"
				           "\nis greater than total number of clusters ( %d )\n",
				           needed_size, size);
			return 1;
		}
		int final_size;
		if ((final_size = final_cluster(clusters,size,needed_size)) == -1){
			clear_clusters(clusters,size);
			return 1;
		}
		size = final_size;
		print_clusters(clusters,size);
		clear_clusters(clusters,size);
	} else {
		fprintf(stderr,"Bad usage.\nUsage: ./proj3 FILE N\n"
			            "FILE = name of .txt file\n"
			            "N = number of clusters you want to create\n");
		return 1;
	}

return 0;
}
