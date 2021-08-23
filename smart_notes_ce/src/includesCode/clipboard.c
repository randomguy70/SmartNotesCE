#include <intce.h>

#include "includes/clipboard.h"

int copyToClipboard(char* data, size_t size, struct clipboard* clipboard) {
	clipboard->data = data;
	clipboard->size = size;
}