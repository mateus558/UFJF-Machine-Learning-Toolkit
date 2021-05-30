//
// Created by mateus on 30/05/2021.
//

#ifndef UFJFMLTK_INT_DLL_H
#define UFJFMLTK_INT_DLL_H

namespace mltk::classifier {
/*double linked list of integers*/
    struct int_dll {
        int index = -1;
        int_dll *prev = nullptr;
        int_dll *next = nullptr;

        int_dll();

        static int_dll *remove(int_dll **node);

        static int_dll *append(int_dll *list);

        static void free(int_dll **head);
    };
    typedef struct int_dll int_dll;
}

#endif //UFJFMLTK_INT_DLL_H
