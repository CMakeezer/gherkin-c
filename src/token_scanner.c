#include "token_scanner.h"

void TokenScanner_delete(TokenScanner* token_scanner) {
    token_scanner->delete_function_(token_scanner);
}
