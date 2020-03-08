#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define ALIVE	true
#define DEAD	false

#define MALE	0
#define FEMALE	1
#define MAX_CHILDREN 10

enum rel_type {MATE, MOTHER, FATHER, CHILD_M, CHILD_F, SIBLING_M, SIBLING_F, GFATHER_M, GMOTHER_M, GFATHER_F, GMOTHER_F};
enum death_type {cancer};
struct relation
{
	struct person*		dst;
	enum rel_type	rel;
	int		rel_conf;
	int		dst_conf;
};

struct person
{
	int known;

	int fu_height;
	int fu_depth;

	uint8_t	sex:1;
	uint8_t	alive:1;

	int	age_dead;
	int	num_children;
	int	num_siblings;
	enum death_type	died_by;

	struct relation*	children[MAX_CHILDREN];
	struct relation*	sibling[MAX_CHILDREN];
	struct relation*	father;
	struct relation*	mother;

	int num_known_mates;
	struct relation*	mate;
};

void create_sibling_f(char* prefix, struct person* self, struct person* sibling_f, int *index);
void create_sibling_m(char* prefix, struct person* self, struct person* sibling_m, int *index);
void create_child_m(char* prefix, struct person* self, struct person* child_m, int* index);
void create_child_f(char* prefix, struct person* self, struct person* child_f, int* index);
void create_mother(char* prefix, struct person* self, struct person* mother);
void create_father(char* prefix, struct person* self, struct person* father);
struct relation* create_rel_and_p(char*, enum rel_type,struct person*);
struct relation* create_rel(char *, enum rel_type,struct person*,struct person*);
void create_fu(char*);
void create_mate(char*, struct person*, struct person*);

struct person* create_individual(char*);

void get_siblings(char* prefix, struct person *in_p);
void get_children(char* prefix, struct person *in_p);
int get_alive(char*, struct person*);
void get_mate(char*, struct person*);
void get_parents(char*, struct person*);
int get_causeof_death(char*);
int get_age_died(char*);
int get_gender(char*);


int main(int argc, char** argv)
{
	char input;
	char prefix[256];
	printf("Press [C] to create family unit:");
	do{ 
		scanf("%c", &input);
	}while(input != 'C');
	
	create_fu("[self]");
	return 0;
}




struct person* create_individual(char* prefix)
{
	struct person* self = (struct person*)malloc(sizeof(struct person));
	self->known	= 1;
	self->sex	= get_gender(prefix);
	self->alive	= get_alive(prefix, self);
	get_mate(prefix, self);
	get_parents(prefix, self);

	return self;
}
void get_siblings(char* prefix, struct person *in_p)
{
	int in_num;
	printf("%s How many siblings exist: ", prefix);
	scanf("%d", &in_num);
	
	while(in_num < 0 || in_num > 10)
	{
		scanf("%d", &in_num);
	}
	in_p->num_siblings = in_num;
	if(in_num > 0)
	{
		for(int i = 0; i < in_p->num_siblings; i++)
		{
			char in_c;
			printf("%s Is sibling {%d} (m)ale or (f)emale: ",prefix, i);
			scanf("%c", &in_c);
			while( in_c != 'f' && in_c != 'm') { scanf("%c", &in_c);}
			if(in_c == 'f')
			{
				in_p->sibling[i] = create_rel_and_p(prefix, SIBLING_F, in_p);
				
			}
			else if(in_c == 'm')
			{
				in_p->sibling[i] = create_rel_and_p(prefix, SIBLING_M, in_p);
			}
		}
	}
}

void get_children(char* prefix, struct person *in_p)
{
	int in_num;
	printf("%s How many children exist: ", prefix);
	scanf("%d", &in_num);
	
	while(in_num < 0 || in_num > 10)
	{
		scanf("%d", &in_num);
	}

	in_p->num_children = in_num;
	if(in_num > 0)
	{
		for(int i = 0; i < in_p->num_children; i++)
		{
			char in_c;
			printf("%s Is child {%d} (m)ale or (f)emale: ",prefix, i);
			scanf("%c", &in_c);
			while( in_c != 'f' && in_c != 'm') { scanf("%c", &in_c);}
			if(in_c == 'f')
			{
				//in_p->children[i] = create_rel_and_p(prefix, CHILD_F, in_p);
			}
			else if(in_c == 'm')
			{
				//in_p->children[i] = create_rel_and_p(prefix, CHILD_M, in_p);
			}
		}
	}
}

void get_mate(char* prefix, struct person* in_p)
{
	char in_char;
	printf("%s 	Does a mate exist (y/n): ", prefix);
	scanf("%c", &in_char);
	while( in_char != 'y' && in_char != 'n')
	{
		scanf("%c", &in_char);
	}

	if(in_char == 'y')
	{
		in_p->num_known_mates = 1;
		get_children(prefix, in_p);
		in_p->mate = create_rel_and_p(prefix, MATE, in_p); 

		
		
	}
	else
	{
		in_p->num_known_mates = 0;
	}
	printf("test %d", in_p->num_known_mates);
}

void get_parents(char* prefix, struct person* self)
{
	char input;
	struct person* father	= (struct person*)malloc(sizeof(struct person));
	struct person* mother	= (struct person*)malloc(sizeof(struct person));
	printf("\r\n Is father known (y/n): ");
	scanf("%c", &input);
	while(input != 'y' && input != 'n')
	{
		scanf("%c", &input);
	}

	if(input == 'y')
	{
		self->father = create_rel_and_p(prefix, FATHER, self);
	}

	printf("\r\n Is mother known (y/n): ");
	scanf("%c", &input);
	while(input != 'y' && input != 'n')
	{
		scanf("%c", &input);
	}

	
	if(input == 'y')
	{
		self->mother = create_rel_and_p(prefix, MOTHER, self);
	}

}

int get_age_died(char* prefix)
{
	fflush(stdin);
	int temp;
	printf("%s 	age at death: ", prefix);
	scanf("%d", &temp);
	while (temp < 0 || temp > 130)
	{
		scanf("%d", &temp);
	}
	return temp;
}

int get_gender(char* prefix)
{
		
	char temp;
	printf("%s 	 gender (m) male (f) female: ", prefix);
	while(temp != 'm' && temp != 'f')
	{
		fflush(stdin);
		scanf("%c", &temp);
	}

	if(temp == 'f')
		return FEMALE;
	else if(temp == 'm')
		return MALE; 
}
int get_alive(char* prefix, struct person* in_person)
{
	fflush(stdin);
	char temp;
	printf("%s 	(a) alive (d) dead:", prefix);
	while(temp != 'a' && temp != 'd')
	{
		scanf("%c", &temp);
	}

	if(temp == 'a')
	{
		temp = ALIVE;
		in_person->age_dead	= -1;
	}
	else if(temp == 'd')
	{
		temp			= DEAD;
		in_person->age_dead	= get_age_died(prefix);
	}
	return temp;
}


void create_fu(char* prefix)
{
	char in_c;
	struct person* self = create_individual(prefix);	
	
	if(self->num_known_mates > 0)
	{	
		self->mate->dst->mate	= create_rel(prefix, MATE, self->mate->dst, self);
	}
}




void create_mate(char* prefix, struct person *self, struct person *mate)
{
	
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[mate]");
	mate->sex	= get_gender(new_prefix);
	mate->alive	= get_alive(new_prefix, mate);
	
	self->mate	= create_rel(new_prefix, MATE, mate, self);
}


void create_mother(char* prefix, struct person* self, struct person* mother)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[mother]");
	mother->sex	= FEMALE;
	mother->num_children = 1;
	mother->alive	= get_alive(new_prefix, mother);
	self->mother	= create_rel(new_prefix, MOTHER, mother, self);	
	
	if(self->num_children != 0)
	{
		for(int i = 0; i < self->num_children; i++)
		{
			if(self->children[i]->dst->sex == MALE)
			{
				mother->children[i] = create_rel(new_prefix, CHILD_M, self->children[i]->dst, mother);
			}
			else if(self->children[i]->dst->sex== FEMALE)
			{
				mother->children[i] = create_rel(new_prefix, CHILD_F, self->children[i]->dst, mother);
			}
		}
	}
}


void create_father(char* prefix, struct person* self, struct person* father)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[father]");
	father->sex	= MALE;
	father->num_children = 1;
	father->alive	= get_alive(new_prefix, father);
	self->father	= create_rel(new_prefix, FATHER, father, self);

	if(self->num_children != 0)
	{
		for(int i = 0; i < self->num_children; i++)
		{
			if(self->children[i]->dst->sex == MALE)
			{
				father->children[i] = create_rel(new_prefix, CHILD_M, self->children[i]->dst, father);
			}
			else if(self->children[i]->dst->sex== FEMALE)
			{
				father->children[i] = create_rel(new_prefix, CHILD_F, self->children[i]->dst, father);
			}
		}
	}
}


void create_child_m(char* prefix, struct person* self, struct person* child_m, int* index)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[child m]");
	child_m->sex	= MALE;
	child_m->num_siblings = self->num_children;
	child_m->alive = get_alive(new_prefix, child_m);
	self->children[*index]	= create_rel(new_prefix, CHILD_M, child_m, self);
	
	*index++;
	if(*index == self->num_children)
	{
		*index = 0;
	}
}

void create_child_f(char* prefix, struct person* self, struct person* child_f, int *index)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[child f]");
	child_f->sex	= FEMALE;
	child_f->num_siblings = self->num_children;
	child_f->alive = get_alive(new_prefix, child_f);
	self->children[*index]	= create_rel(new_prefix, CHILD_F, child_f, self);
	*index++;
	if(*index == self->num_children)
	{
		*index = 0;
	}
}


void create_sibling_f(char* prefix, struct person* self, struct person* sibling_f, int *index)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[sister]");
	sibling_f->sex	= FEMALE;
	sibling_f->num_siblings = self->num_siblings;
	sibling_f->alive = get_alive(new_prefix, sibling_f);
	self->sibling[*index]	= create_rel(new_prefix, SIBLING_F, sibling_f, self);
	*index++;
	if(*index == self->num_children)
	{
		*index = 0;
	}

	sibling_f->father	= create_rel(new_prefix, FATHER, self->father->dst, sibling_f);
	sibling_f->mother	= create_rel(new_prefix, MOTHER, self->mother->dst, sibling_f);
}

void create_sibling_m(char* prefix, struct person* self, struct person* sibling_m, int *index)
{
	char new_prefix[256];
	sprintf(new_prefix, "%s%s", prefix, "->[brother]");
	sibling_m->sex	= MALE;
	sibling_m->num_siblings = self->num_siblings;
	sibling_m->alive = get_alive(new_prefix, sibling_m);
	self->sibling[*index]	= create_rel(new_prefix, SIBLING_M, sibling_m, self);
	sibling_m->father	= create_rel(new_prefix, FATHER, self->father->dst, sibling_m);
	sibling_m->mother	= create_rel(new_prefix, MOTHER, self->mother->dst, sibling_m);	
	*index++;
	if(*index == self->num_children)
	{
		*index = 0;
	}
}




struct relation* create_rel_and_p(char* prefix, enum rel_type type, struct person *self)
{
	static int child_index = 0;
	static int sibling_index = 0;
	struct relation* new_relation = (struct relation*)malloc(sizeof(struct relation));
	struct person* new_person = (struct person*)malloc(sizeof(struct person));
	new_person->known = 1;	
	switch(type)
	{
		case MATE:
			create_mate(prefix, self, new_person);
			break;

		case MOTHER:
			create_father(prefix, self, new_person);
			break;

		case FATHER:
			create_father(prefix, self, new_person);
			break;

		case CHILD_M:
			create_child_m(prefix, self, new_person, &child_index);
			break;

		case CHILD_F:			
			create_child_f(prefix, self, new_person, &child_index);
			break;

		case SIBLING_M:
			create_sibling_m(prefix, self, new_person, &sibling_index);
			break;

		case SIBLING_F:
			create_sibling_f(prefix, self, new_person, &sibling_index);
			break;

		case GMOTHER_F:
			break;

		case GFATHER_F:
			break;

		case GMOTHER_M:
			break;

		case GFATHER_M:
			break;
		default:
			break;
	}
	return new_relation;
}

struct relation* create_rel(char *prefix, enum rel_type type, struct person *to, struct person *from)
{
	struct relation* new_rel = (struct relation*)malloc(sizeof(struct relation));

	new_rel->rel	= type;
	new_rel->dst	= to;

	switch(type)
	{
		case MATE:
			new_rel->rel_conf	= 1;
			new_rel->dst_conf	= 1;
			break;

		case MOTHER:
			break;

		case FATHER:
			break;

		case CHILD_M:
			break;

		case CHILD_F:
			break;

		case SIBLING_M:
			break;

		case SIBLING_F:
			break;

		case GMOTHER_F:
			break;

		case GFATHER_F:
			break;

		case GMOTHER_M:
			break;

		case GFATHER_M:
			break;
		default:
			break;
	}

	return new_rel;
}

