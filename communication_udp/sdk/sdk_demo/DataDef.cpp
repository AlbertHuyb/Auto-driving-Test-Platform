#include "DataDef.h"
#include <stdlib.h>

extern "C" {
	void a_set_empty(void *a_set_of_x)
	{
		a_anonymous_set_ *as = _A_SET_FROM_VOID(a_set_of_x);

		if(as) {
			if(as->array > 0){
				for(int i=0;i<as->count;i++){
					if(as->array[i] > 0)
						free(as->array[i]);
				}
				free(as->array);
			}
		}
	}
}
