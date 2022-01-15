/* On inclut l'interface publique */
#include "mem.h"
#include"malloc_stub.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

/* Définition de l'alignement recherché
 * Avec gcc, on peut utiliser __BIGGEST_ALIGNMENT__
 * sinon, on utilise 16 qui conviendra aux plateformes qu'on cible
 */
#ifdef __BIGGEST_ALIGNMENT__
#define ALIGNMENT __BIGGEST_ALIGNMENT__
#else
#define ALIGNMENT 16ss
#endif


/* structure placée au début de la zone de l'allocateur

   Elle contient toutes les variables globales nécessaires au
   fonctionnement de l'allocateur

   Elle peut bien évidemment être complétée
*/
struct allocator_header {
    size_t memory_size;
	mem_fit_function_t* fit;
	struct fb* first;
};


/* La seule variable globale autorisée
 * On trouve à cette adresse le début de la zone à gérer
 * (et une structure 'struct allocator_header)
 */
static void* memory_addr;

struct fb* get_head()
{
	return memory_addr + sizeof(struct allocator_header);
};


static inline void *get_system_memory_addr() {
	return memory_addr;
}

static inline struct allocator_header *get_header() {
	struct allocator_header *h;
	h = get_system_memory_addr();
	return h;
}

static inline size_t get_system_memory_size() {
	return get_header()->memory_size;
}

struct fb {
	size_t size;
	struct fb* next;
	int isFree;
};

int hasNext(struct fb* block)
{
	return block->next != NULL ? 1 : 0;
}

struct fb* getNext(struct fb* block)
{
	return block->next;
}

void mem_init(void* mem, size_t taille)
{
	memory_addr = mem;
	*(size_t*)memory_addr = taille;

	//On définit la fonction fit à utiliser
	mem_fit(&mem_fit_first);

	get_header()->memory_size = taille;
	
	struct fb* firstb = get_head();
	firstb->isFree = 1;
	firstb->size = taille - sizeof(struct allocator_header) - sizeof(struct fb);
	firstb->next = NULL;
	//Création d'un unique bloc de mémoire libre contenant toute la mémoire disponible
	get_header()->first = firstb;

	/* On vérifie qu'on a bien enregistré les infos et qu'on
	 * sera capable de les récupérer par la suite
	 */
	assert(mem == get_system_memory_addr());
	assert(taille == get_system_memory_size());
}

void mem_show(void (*print)(void *, size_t, int)) {
	struct fb* bloc = get_head();
	while ((void*)bloc < get_system_memory_addr() + get_system_memory_size())
	{

		print(bloc, bloc->size, bloc->isFree);
		bloc = (void*)bloc + sizeof(struct fb) + bloc->size;
	}
}

void mem_fit(mem_fit_function_t *f) {
	get_header()->fit = f;
}


size_t Taille(size_t taille)
{
	return taille;
}

void* mem_alloc(size_t taille) {
	size_t ttaille = Taille(taille);

	// __attribute__((unused)) /* juste pour que gcc compile ce squelette avec -Werror */
	struct fb* fb = get_header()->fit(get_header()->first, ttaille);
	if (fb == NULL) return NULL;

	if (ttaille != fb->size)
	{
		struct fb* newBloc = (void*)fb + sizeof(struct fb) + ttaille;
		newBloc->isFree = 1;
		newBloc->next = NULL;
		newBloc->size =  fb->size - ttaille - sizeof(struct fb);
		if (fb == get_header()->first)
		{
			get_header()->first = newBloc;
		}
	}

	fb->size = ttaille;
	fb->isFree = 0;
	fb->next = NULL;

	return fb + sizeof(struct fb);
}


struct fb* findPrevFb(void* mem)
{ 
	struct fb* bloc = get_header()->first;
	if (bloc == mem){
		return NULL;
	}
	
	while ((void*)bloc->next < (void*)mem){
		if (bloc->next == NULL) break;
		bloc = getNext(bloc);
	}
	if ((void*)bloc > (void*)mem)
	{
		return NULL;
	}
	
	return bloc;
}

void FusionFB(struct fb* newBloc)
{
	if (newBloc == NULL) return;

	struct fb* prev = findPrevFb(newBloc);
	if( prev + prev->size + sizeof(struct fb)  == newBloc){
		prev->next = newBloc->next;
		prev->size += sizeof(struct fb) + newBloc->size;		
	}
	FusionFB(prev->next);
}

int isFree(struct fb* bloc)
{

	if (bloc == get_header()->first)
		return 1;


	if(findPrevFb(bloc)->next == bloc)
		return 1;
	return 0;
}


void mem_free(void* mem) {
	struct fb* bloc = mem;

	if (bloc->isFree)
	{
		return;
	}
	
	bloc->isFree = 1;
	if (findPrevFb(bloc) == NULL)
	{
		bloc->next = get_header()->first;
		get_header()->first = bloc;
	}
	
	bloc->next = findPrevFb(bloc)->next;
	findPrevFb(bloc)->next = bloc;
	FusionFB(bloc);
}


struct fb* mem_fit_first(struct fb *list, size_t size) {
	
	struct fb* block = list;
	while (1)
	{
		if (block->size >= size + sizeof(struct fb))
		{
			return block;
		}
		if(hasNext(block))
		{
			block = getNext(block);
		}
		if (block->next == NULL)
		{
			break;
		}
	}
	return NULL;
}

/* Fonction à faire dans un second temps
 * - utilisée par realloc() dans malloc_stub.c
 * - nécessaire pour remplacer l'allocateur de la libc
 * - donc nécessaire pour 'make test_ls'
 * Lire malloc_stub.c pour comprendre son utilisation
 * (ou en discuter avec l'enseignant)
 */
size_t mem_get_size(void *zone) {
	/* zone est une adresse qui a été retournée par mem_alloc() */

	/* la valeur retournée doit être la taille maximale que
	 * l'utilisateur peut utiliser dans cette zone */
	return 0;
}


/* Fonctions facultatives
 * autres stratégies d'allocation
 */
struct fb* mem_fit_best(struct fb *list, size_t size) {
	return NULL;
}

struct fb* mem_fit_worst(struct fb *list, size_t size) {
	return NULL;
}



