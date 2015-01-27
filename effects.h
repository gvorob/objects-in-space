#ifndef EFFECTS_H
#define EFFECTS_H

struct _gamestate_struct;

typedef enum _effect_type{
	ET_HIT_SPARK,
} effect_type;


struct spark_effect_struct {
	float x, y;
	float vx, vy;
	int ttl;
};

union effects_union {
	struct spark_effect_struct spark;
};


typedef struct _effect_struct {
	union effects_union data;
	effect_type type;
  	struct _effect_struct *next;
} effect_struct;



//Generate sparks from a hit
void effect_hit(int x, int y, struct _gamestate_struct *gs);

//Mallocs, copies over, inserts into linked list
void create_effect(effect_struct *es, struct _gamestate_struct *gs);

void update_effects(struct _gamestate_struct *gs);
void render_effects(int client_index, struct _gamestate_struct *gs);

#endif
