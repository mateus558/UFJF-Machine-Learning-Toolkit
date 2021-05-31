//
// Created by mateus on 30/05/2021.
//

#include <iostream>
#include "ufjfmltk/classifier/int_dll.h"

namespace mltk::classifier {

/*----------------------------------------------------------*
* Created a double linked list of ints                     *
*----------------------------------------------------------*/

    int_dll::int_dll() {
        this->next = nullptr;
        this->index = -1;
        this->prev = nullptr;
    }

/*----------------------------------------------------------*
* Removes an element from a double linked list            *
*----------------------------------------------------------*/

    int_dll *int_dll::remove(int_dll **node) {
        int_dll *ret = nullptr;

        if ((*node) == nullptr) return nullptr;

        /*remove items from list*/
        ret = (*node)->prev;

        /*fix reference one*/
        if ((*node)->prev != nullptr)
            (*node)->prev->next = (*node)->next;

        /*fix reference two*/
        if ((*node)->next != nullptr)
            (*node)->next->prev = (*node)->prev;

        delete *node;
        (*node) = nullptr;

        return ret;
    }

/*----------------------------------------------------------*
* Appends a new node after list                            *
*----------------------------------------------------------*/

    int_dll *int_dll::append(int_dll *list) {
        int_dll *tmp = nullptr;

        /*error check*/
        if (list == nullptr) {
            std::cerr << "Error in int linked list\n";
            return nullptr;
        }

        /*save old next*/
        tmp = list->next;

        /*new node*/
        list->next = new int_dll;
        if (list == nullptr) {
            std::cerr << "Error: Out of memory\n";
            return nullptr;
        }

        /*reference fixing*/
        list->next->prev = list;
        list->next->next = tmp;
        if (tmp != nullptr) tmp->prev = list->next;

        /*finishing up*/
        list = list->next;
        list->index = -1;

        /*returning*/
        return list;
    }

/*----------------------------------------------------------*
* Clears linked list                                       *
*----------------------------------------------------------*/

    void int_dll::free(int_dll **head) {
        int_dll *list = nullptr;
        int_dll *tmpl = nullptr;

        if (*head == nullptr) return;

        list = *head;
        while (list != nullptr) {
            tmpl = list;
            list = list->next;
            delete tmpl;
        }
        *head = nullptr;
    }
}