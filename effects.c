#include "effects.h"
#include "main_structs.h"


void effect_hit(int x, int y, struct _gamestate_struct *gs) {
	effect_struct temp_es;
	int i;

	for(i = 0; i < 15; i++) {
		//init shot
		temp_es.type = ET_HIT_SPARK;

		temp_es.data.spark.x = x;
		temp_es.data.spark.y = y;
		temp_es.data.spark.vx = rand_float(-1,1);
		temp_es.data.spark.vy = rand_float(-1,1);
		temp_es.data.spark.ttl = rand_int(1,20);
		

		//create and add to LL
		create_effect(&temp_es, gs);
	}
}

void create_effect(effect_struct *es, gamestate_struct *gs) {
	effect_struct *new_es;
	
	//malloc a new struct
	new_es = (effect_struct *) malloc(sizeof(effect_struct));

	//copy over data
	memcpy(new_es, es, sizeof(effect_struct));

	//Insert into linked list
	new_es->next = gs->effects_list.next;
	gs->effects_list.next = new_es;

}

void update_effects(struct _gamestate_struct *gs) {
	effect_struct *esp;

	for(esp = &(gs->effects_list); esp->next != NULL; ) {
		union effects_union *data = &esp->next->data;
		char has_died = 0;

		switch(esp->next->type) {
			case ET_HIT_SPARK:
				data->spark.x += data->spark.vx;
				data->spark.y += data->spark.vy;
				data->spark.ttl--;

				if(data->spark.ttl == 0)
					has_died = 1;
				break;
			default:
				err(-1, "Something went horribly wrong in update_effects");
		}

			//if dying
		if(has_died) {
			esp->next = esp->next->next;
		} else {
			//Walk the list manually so that we can delete things as we go
			esp = esp->next;
		}
	}

}

void render_effects(int client_index, struct _gamestate_struct *gs) {
	effect_struct *esp;
	char *rp;

	int temp_x, temp_y;

	rp = gs->clients[client_index].render.render_data;

	for(esp = &(gs->effects_list); esp->next != NULL; esp = esp->next) {
		union effects_union *data = &esp->next->data;

		switch(esp->next->type) {
			case ET_HIT_SPARK:
				temp_x = (data->spark.x + 0.5f);
				temp_y = (data->spark.y + 0.5f);
				if(temp_x < 0 || temp_y < 0 || temp_x >= gs->shipstate.tiles.width || temp_y >= gs->shipstate.tiles.height)
					break;
				rp[SCREEN_INDEX(temp_x, temp_y)] = '~';
				break;
			default:
				err(-1, "Something went horribly wrong in update_effects");
		}
	}

}
