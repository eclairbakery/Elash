#pragma once

typedef enum ElAstTopLevelType {
    // TODO: nothing for now
    TODO,
} ElAstTopLevelType;

typedef struct ElAstTopLevelNode {
    ElAstTopLevelType type;
    union {
        // TODO: nothing for now
    } as;
} ElAstTopLevelNode;
