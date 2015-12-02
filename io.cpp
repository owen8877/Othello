#include "base.h"
#include "element.h"
#include "io.h"

Piece getPieceFromIO(bool side){
    printf("Please input your piece :\n");
    char input[2];
    scanf("%s", input);
    int x = 0, y = 0;
    if ((input[0]>='a')&&(input[0]<='h')) y = input[0] - 'a' + 1;
    if ((input[0]>='A')&&(input[0]<='H')) y = input[0] - 'A' + 1;
    if ((input[1]>='1')&&(input[1]<='8')) x = input[1] - '0';
    return Piece(x, y, getSideTag(side));
}
