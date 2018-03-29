
#include <stdio.h>
#include <stdlib.h>
#include "json.h"


static void print_depth_shift(FILE* fp, int depth) {
	int j;
	for (j = 0; j < depth; j++) {
		fprintf(fp, " ");
	}
}

static void process_value(FILE* fp, json_value* value, int depth);

static void process_object(FILE* fp, json_value* value, int depth) {
	int length, x;
	if (value == NULL) {
		return;
	}
	length = value->u.object.length;
	for (x = 0; x < length; x++) {
		print_depth_shift(fp, depth);
		fprintf(fp, "object[%d].name = %s\n", x, value->u.object.values[x].name);
		process_value(fp, value->u.object.values[x].value, depth + 1);
	}
}

static void process_array(FILE* fp, json_value* value, int depth) {
	int length, x;
	if (value == NULL) {
		return;
	}
	length = value->u.array.length;
	fprintf(fp, "array\n");
	for (x = 0; x < length; x++) {
		process_value(fp, value->u.array.values[x], depth);
	}
}

static void process_value(FILE* fp, json_value* value, int depth) {
	if (value == NULL) {
		return;
	}
	if (value->type != json_object) {
		print_depth_shift(fp, depth);
	}
	switch (value->type) {
	case json_none:
		fprintf(fp, "none\n");
		break;
	case json_object:
		process_object(fp, value, depth + 1);
		break;
	case json_array:
		process_array(fp, value, depth + 1);
		break;
	case json_integer:
		fprintf(fp, "int: %ld\n", value->u.integer);
		break;
	case json_double:
		fprintf(fp, "double: %f\n", value->u.dbl);
		break;
	case json_string:
		fprintf(fp, "string: %s\n", value->u.string.ptr);
		break;
	case json_boolean:
		fprintf(fp, "bool: %d\n", value->u.boolean);
		break;
	default:
		break;
	}
}

void json_print_value(FILE* fp, json_value* value)
{
	process_value(fp, value, 0);
}
