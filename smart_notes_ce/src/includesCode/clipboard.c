#include <intce.h>

#include "includes/clipboard.h"

int copyToClipboard(char* data, unsigned int size, struct clipboard* clipboard) {
	clipboard->data = data;
	clipboard->size = size;
}