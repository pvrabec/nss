/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape security libraries.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1994-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Sun Microsystems
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/*
 * pkix_list.c
 *
 * List Object Functions
 *
 */

#include "pkix_list.h"

/* --Private-Functions-------------------------------------------- */

/*
 * FUNCTION: pkix_List_Create_Internal
 * DESCRIPTION:
 *
 *  Creates a new List, using the Boolean value of "isHeader" to determine
 *  whether the new List should be a header, and stores it at "pList". The
 *  List is initially empty and holds no items. To initially add items to
 *  the List, use PKIX_List_AppendItem.
 *
 * PARAMETERS:
 *  "isHeader"
 *      Boolean value indicating whether new List should be a header.
 *  "pList"
 *      Address where object pointer will be stored. Must be non-NULL.
 *  "plContext"
 *      Platform-specific context pointer.
 * THREAD SAFETY:
 *  Thread Safe (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds.
 *  Returns a Fatal Error if the function fails in an unrecoverable way.
 */
static PKIX_Error *
pkix_List_Create_Internal(
        PKIX_Boolean isHeader,
        PKIX_List **pList,
        void *plContext)
{
        PKIX_List *list = NULL;

        PKIX_ENTER(LIST, "pkix_List_Create_Internal");
        PKIX_NULLCHECK_ONE(pList);

        PKIX_CHECK(PKIX_PL_Object_Alloc
                    (PKIX_LIST_TYPE,
                    ((PKIX_UInt32)(sizeof (PKIX_List))),
                    (PKIX_PL_Object **)&list, plContext),
                    "Error Creating List Item");

        list->item = NULL;
        list->next = NULL;
        list->immutable = PKIX_FALSE;
        list->length = 0;
        list->isHeader = isHeader;

        *pList = list;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Destroy
 * (see comments for PKIX_PL_DestructorCallback in pkix_pl_system.h)
 */
static PKIX_Error *
pkix_List_Destroy(
        PKIX_PL_Object *object,
        void *plContext)
{
        PKIX_List *list = NULL;

        PKIX_ENTER(LIST, "pkix_List_Destroy");
        PKIX_NULLCHECK_ONE(object);

        /* Check that this object is a list */
        PKIX_CHECK(pkix_CheckType(object, PKIX_LIST_TYPE, plContext),
                    "Object is not a list");

        list = (PKIX_List *)object;

        /* We have a valid list. DecRef its item and recurse on next */
        PKIX_DECREF(list->item);
        PKIX_DECREF(list->next);
        list->immutable = PKIX_FALSE;
        list->length = 0;
        list->isHeader = PKIX_FALSE;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_ToString_Helper
 * DESCRIPTION:
 *
 *  Helper function that creates a string representation of the List pointed
 *  to by "list" and stores its address in the object pointed to by "pString".
 *
 * PARAMETERS
 *  "list"
 *      Address of List whose string representation is desired.
 *      Must be non-NULL.
 *  "pString"
 *      Address of object pointer's destination. Must be non-NULL.
 *  "plContext"
 *      Platform-specific context pointer.
 * THREAD SAFETY:
 *  Conditionally Thread Safe
 *      (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds.
 *  Returns a List Error if the function fails in a non-fatal way.
 *  Returns a Fatal Error if the function fails in an unrecoverable way.
 */
static PKIX_Error *
pkix_List_ToString_Helper(
        PKIX_List *list,
        PKIX_PL_String **pString,
        void *plContext)
{
        PKIX_PL_String *itemString = NULL;
        PKIX_PL_String *nextString = NULL;
        PKIX_PL_String *format = NULL;
        PKIX_Boolean empty;

        PKIX_ENTER(LIST, "pkix_List_ToString_Helper");
        PKIX_NULLCHECK_TWO(list, pString);

        /* special case when list is the header */
        if (list->isHeader){

                PKIX_CHECK(PKIX_List_IsEmpty(list, &empty, plContext),
                            "PKIX_List_IsEmpty failed");

                if (empty){
                        PKIX_CHECK(PKIX_PL_String_Create
                                    (PKIX_ESCASCII,
                                    "EMPTY",
                                    0,
                                    &itemString,
                                    plContext),
                                    "Error creating item string");
                        (*pString) = itemString;
                        PKIX_DEBUG_EXIT(LIST);
                        return (NULL);
                } else {
                        PKIX_CHECK(pkix_List_ToString_Helper
                                    (list->next, &itemString, plContext),
                                    "pkix_List_ToString_Helper failed");
                }

                /* Create a string object from the format */
                PKIX_CHECK(PKIX_PL_String_Create
                            (PKIX_ESCASCII, "%s", 0, &format, plContext),
                            "PKIX_PL_String_Create failed");

                PKIX_CHECK(PKIX_PL_Sprintf
                            (pString, plContext, format, itemString),
                            "PKIX_PL_Sprintf failed");
        } else {
                /* Get a string for this list's item */
                if (list->item == NULL) {
                        PKIX_CHECK(PKIX_PL_String_Create
                                    (PKIX_ESCASCII,
                                    "(null)",
                                    0,
                                    &itemString,
                                    plContext),
                                    "PKIX_PL_String_Create failed");
                } else {
                        PKIX_CHECK(PKIX_PL_Object_ToString
                                    ((PKIX_PL_Object*)list->item,
                                    &itemString,
                                    plContext),
                                    "PKIX_PL_Object_ToString failed");
                }
                if (list->next == NULL) {
                        /* Just return the itemstring */
                        (*pString) = itemString;
                        PKIX_DEBUG_EXIT(LIST);
                        return (NULL);
                }

                /* Recursive call to get string for this list's next pointer */
                PKIX_CHECK(pkix_List_ToString_Helper
                            (list->next, &nextString, plContext),
                            "pkix_List_ToString_Helper failed");

                /* Create a string object from the format */
                PKIX_CHECK(PKIX_PL_String_Create
                            (PKIX_ESCASCII,
                            "%s, %s",
                            0,
                            &format,
                            plContext),
                            "PKIX_PL_String_Create failed");

                PKIX_CHECK(PKIX_PL_Sprintf
                            (pString,
                            plContext,
                            format,
                            itemString,
                            nextString),
                            "PKIX_PL_Sprintf failed");
        }

cleanup:

        PKIX_DECREF(itemString);
        PKIX_DECREF(nextString);
        PKIX_DECREF(format);

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_ToString
 * (see comments for PKIX_PL_ToStringCallback in pkix_pl_system.h)
 */
static PKIX_Error *
pkix_List_ToString(
        PKIX_PL_Object *object,
        PKIX_PL_String **pString,
        void *plContext)
{
        PKIX_List *list = NULL;
        PKIX_PL_String *listString = NULL;
        PKIX_PL_String *format = NULL;

        PKIX_ENTER(LIST, "pkix_List_ToString");
        PKIX_NULLCHECK_TWO(object, pString);

        PKIX_CHECK(pkix_CheckType(object, PKIX_LIST_TYPE, plContext),
                    "Object is not a list");

        list = (PKIX_List *)object;

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        PKIX_CHECK(pkix_List_ToString_Helper(list, &listString, plContext),
                    "pkix_List_ToString Helper failed");

        PKIX_CHECK(PKIX_PL_String_Create
                    (PKIX_ESCASCII, "(%s)", 0, &format, plContext),
                    "PKIX_PL_String_Create failed");

        PKIX_CHECK(PKIX_PL_Sprintf(pString, plContext, format, listString),
                    "PKIX_PL_Sprintf failed");

cleanup:

        PKIX_DECREF(listString);
        PKIX_DECREF(format);

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Equals
 * (see comments for PKIX_PL_EqualsCallback in pkix_pl_system.h)
 */
static PKIX_Error *
pkix_List_Equals(
        PKIX_PL_Object *first,
        PKIX_PL_Object *second,
        PKIX_Boolean *pResult,
        void *plContext)
{
        PKIX_UInt32 secondType;
        PKIX_Boolean cmpResult;
        PKIX_List *firstList = NULL;
        PKIX_List *secondList = NULL;
        PKIX_UInt32 firstLength = 0;
        PKIX_UInt32 secondLength = 0;
        PKIX_PL_Object *firstItem = NULL;
        PKIX_PL_Object *secondItem = NULL;
        PKIX_UInt32 i = 0;

        PKIX_ENTER(LIST, "pkix_List_Equals");
        PKIX_NULLCHECK_THREE(first, second, pResult);

        /* test that first is a List */
        PKIX_CHECK(pkix_CheckType(first, PKIX_LIST_TYPE, plContext),
                    "First Argument is not a List");

        /*
         * Since we know first is a List, if both references are
         * identical, they must be equal
         */
        if (first == second){
                *pResult = PKIX_TRUE;
                goto cleanup;
        }

        /*
         * If second isn't a List, we don't throw an error.
         * We simply return a Boolean result of FALSE
         */
        *pResult = PKIX_FALSE;
        PKIX_CHECK(PKIX_PL_Object_GetType(second, &secondType, plContext),
                    "Could not get type of second argument");
        if (secondType != PKIX_LIST_TYPE) goto cleanup;

        firstList = (PKIX_List *)first;
        secondList = (PKIX_List *)second;

        if ((!firstList->isHeader) && (!secondList->isHeader)){
                PKIX_ERROR("Input Lists must be list headers");
        }

        firstLength = firstList->length;
        secondLength = secondList->length;

        cmpResult = PKIX_FALSE;
        if (firstLength == secondLength){
                for (i = 0, cmpResult = PKIX_TRUE;
                    ((i < firstLength) && cmpResult);
                    i++){
                        PKIX_CHECK(PKIX_List_GetItem
                                    (firstList, i, &firstItem, plContext),
                                    "PKIX_List_GetItem failed");

                        PKIX_CHECK(PKIX_List_GetItem
                                    (secondList, i, &secondItem, plContext),
                                    "PKIX_List_GetItem failed");

                        if ((!firstItem && secondItem) ||
                            (firstItem && !secondItem)){
                                        cmpResult = PKIX_FALSE;
                        } else if (!firstItem && !secondItem){
                                continue;
                        } else {
                                PKIX_CHECK(PKIX_PL_Object_Equals
                                            (firstItem,
                                            secondItem,
                                            &cmpResult,
                                            plContext),
                                            "PKIX_PL_Object_Equals failed");

                                PKIX_DECREF(firstItem);
                                PKIX_DECREF(secondItem);
                        }
                }
        }

        *pResult = cmpResult;

cleanup:

        PKIX_DECREF(firstItem);
        PKIX_DECREF(secondItem);

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Hashcode
 * (see comments for PKIX_PL_HashcodeCallback in pkix_pl_system.h)
 */
static PKIX_Error *
pkix_List_Hashcode(
        PKIX_PL_Object *object,
        PKIX_UInt32 *pHashcode,
        void *plContext)
{
        PKIX_List *list = NULL;
        PKIX_PL_Object *element = NULL;
        PKIX_UInt32 hash = 0;
        PKIX_UInt32 tempHash = 0;
        PKIX_UInt32 length, i;

        PKIX_ENTER(LIST, "pkix_List_Hashcode");
        PKIX_NULLCHECK_TWO(object, pHashcode);

        PKIX_CHECK(pkix_CheckType(object, PKIX_LIST_TYPE, plContext),
                    "Object is not a list");

        list = (PKIX_List *)object;

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        length = list->length;

        for (i = 0; i < length; i++){
                PKIX_CHECK(PKIX_List_GetItem(list, i, &element, plContext),
                            "PKIX_List_GetItem failed");

                if (!element){
                        tempHash = 100;
                } else {
                        PKIX_CHECK(PKIX_PL_Object_Hashcode
                                    (element, &tempHash, plContext),
                                    "pkix_List_Hashcode failed");
                }

                hash = 31 * hash + tempHash;

                PKIX_DECREF(element);
        }

        *pHashcode = hash;

cleanup:

        PKIX_DECREF(element);
        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Duplicate
 * (see comments for PKIX_PL_DuplicateCallback in pkix_pl_system.h)
 */
static PKIX_Error *
pkix_List_Duplicate(
        PKIX_PL_Object *object,
        PKIX_PL_Object **pNewObject,
        void *plContext)
{
        PKIX_List *list = NULL;
        PKIX_List *listDuplicate = NULL;

        PKIX_ENTER(LIST, "pkix_List_Duplicate");
        PKIX_NULLCHECK_TWO(object, pNewObject);

        PKIX_CHECK(pkix_CheckType(object, PKIX_LIST_TYPE, plContext),
                    "Object is not a list");

        list = (PKIX_List *)object;

        if (list->immutable){
                PKIX_CHECK(pkix_duplicateImmutable
                            (object, pNewObject, plContext),
                            "pkix_duplicateImmutable failed");
        } else {

                PKIX_CHECK(pkix_List_Create_Internal
                            (list->isHeader, &listDuplicate, plContext),
                            "pkix_List_Create_Internal failed");

                listDuplicate->length = list->length;

                PKIX_INCREF(list->item);
                listDuplicate->item = list->item;

                if (list->next == NULL){
                        listDuplicate->next = NULL;
                } else {
                        /* Recursively Duplicate list */
                        PKIX_CHECK(pkix_List_Duplicate
                                    ((PKIX_PL_Object *)list->next,
                                    (PKIX_PL_Object **)&listDuplicate->next,
                                    plContext),
                                    "pkix_List_Duplicate failed");
                }

                *pNewObject = (PKIX_PL_Object *)listDuplicate;
        }

cleanup:

        if (PKIX_ERROR_RECEIVED){
                PKIX_DECREF(listDuplicate);
        }

        PKIX_RETURN(LIST);
}


/*
 * FUNCTION: pkix_List_GetElement
 * DESCRIPTION:
 *
 *  Copies the "list"'s element at "index" into "element". The input List must
 *  be the header of the List (as opposed to being an element of the List). The
 *  index counts from zero and must be less than the List's length. This
 *  function does NOT increment the reference count of the List element since
 *  the returned element's reference will not be stored by the calling
 *  function.
 *
 * PARAMETERS:
 *  "list"
 *      Address of List (must be header) to get element from. Must be non-NULL.
 *  "index"
 *      Index of list to get element from. Must be less than List's length.
 *  "pElement"
 *      Address where object pointer will be stored. Must be non-NULL.
 *  "plContext"
 *      Platform-specific context pointer.
 * THREAD SAFETY:
 *  Conditionally Thread Safe
 *      (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds.
 *  Returns a Fatal Error if the function fails in an unrecoverable way.
 */
static PKIX_Error *
pkix_List_GetElement(
        PKIX_List *list,
        PKIX_UInt32 index,
        PKIX_List **pElement,
        void *plContext)
{
        PKIX_List *iterator = NULL;
        PKIX_UInt32 length;
        PKIX_UInt32 position = 0;

        PKIX_ENTER(LIST, "pkix_List_GetElement");
        PKIX_NULLCHECK_TWO(list, pElement);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        length = list->length;

        if (index >= length) {
                PKIX_ERROR("Index out of bounds");
        }

        for (iterator = list; position++ <= index; iterator = iterator->next)
                ;

        (*pElement) = iterator;

cleanup:

        PKIX_RETURN(LIST);
}


/*
 * FUNCTION: pkix_List_RegisterSelf
 * DESCRIPTION:
 *  Registers PKIX_LIST_TYPE and its related functions with systemClasses[]
 * THREAD SAFETY:
 *  Not Thread Safe - for performance and complexity reasons
 *
 *  Since this function is only called by PKIX_PL_Initialize, which should
 *  only be called once, it is acceptable that this function is not
 *  thread-safe.
 */
PKIX_Error *
pkix_List_RegisterSelf(void *plContext)
{
        extern pkix_ClassTable_Entry systemClasses[PKIX_NUMTYPES];
        pkix_ClassTable_Entry entry;

        PKIX_ENTER(LIST, "pkix_List_RegisterSelf");

        entry.description = "List";
        entry.destructor = pkix_List_Destroy;
        entry.equalsFunction = pkix_List_Equals;
        entry.hashcodeFunction = pkix_List_Hashcode;
        entry.toStringFunction = pkix_List_ToString;
        entry.comparator = NULL;
        entry.duplicateFunction = pkix_List_Duplicate;

        systemClasses[PKIX_LIST_TYPE] = entry;

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Contains
 * DESCRIPTION:
 *
 *  Checks a List pointed to by "list", to determine whether it includes
 *  an entry that is equal to the Object pointed to by "object", and stores
 *  the result in "pFound".
 *
 * PARAMETERS:
 *  "list"
 *      List to be searched; may be empty; must be non-NULL
 *  "object"
 *      Object to be checked for; must be non-NULL
 *  "pFound"
 *      Address where the result of the search will be stored. Must
 *      be non-NULL
 *  "plContext"
 *      platform-specific context pointer
 * THREAD SAFETY:
 *  Thread Safe (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds
 *  Returns a Validate Error if the functions fails in a non-fatal way
 *  Returns a Fatal Error if the function fails in an unrecoverable way
 */
PKIX_Error *
pkix_List_Contains(
        PKIX_List *list,
        PKIX_PL_Object *object,
        PKIX_Boolean *pFound,
        void *plContext)
{
        PKIX_PL_Object *current = NULL;
        PKIX_UInt32 numEntries = 0;
        PKIX_UInt32 index = 0;
        PKIX_Boolean match = PKIX_FALSE;

        PKIX_ENTER(LIST, "pkix_List_Contains");
        PKIX_NULLCHECK_THREE(list, object, pFound);

        PKIX_CHECK(PKIX_List_GetLength(list, &numEntries, plContext),
                "PKIX_List_GetLength failed");

        for (index = 0; index < numEntries; index++) {
                PKIX_CHECK(PKIX_List_GetItem
                        (list, index, &current, plContext),
                        "PKIX_List_GetItem failed");

                if (current) {
                        PKIX_CHECK(PKIX_PL_Object_Equals
                                (object, current, &match, plContext),
                                "PKIX_PL_Object_Equals failed");

                        PKIX_DECREF(current);
                }

                if (match) {
                        break;
                }
        }

        *pFound = match;

cleanup:

        PKIX_DECREF(current);
        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_Remove
 * DESCRIPTION:
 *
 *  Traverses the List pointed to by "list", to find and delete an entry
 *  that is equal to the Object pointed to by "object". If no such entry
 *  is found the function does not return an error.
 *
 * PARAMETERS:
 *  "list"
 *      List to be searched; may be empty; must be non-NULL
 *  "object"
 *      Object to be checked for and deleted, if found; must be non-NULL
 *  "plContext"
 *      platform-specific context pointer
 * THREAD SAFETY:
 *  Thread Safe (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds
 *  Returns a Validate Error if the functions fails in a non-fatal way
 *  Returns a Fatal Error if the function fails in an unrecoverable way
 */
PKIX_Error *
pkix_List_Remove(
        PKIX_List *list,
        PKIX_PL_Object *object,
        void *plContext)
{
        PKIX_PL_Object *current = NULL;
        PKIX_UInt32 numEntries = 0;
        PKIX_UInt32 index = 0;
        PKIX_Boolean match = PKIX_FALSE;

        PKIX_ENTER(LIST, "pkix_List_Remove");
        PKIX_NULLCHECK_TWO(list, object);

        PKIX_CHECK(PKIX_List_GetLength(list, &numEntries, plContext),
                "PKIX_List_GetLength failed");

        for (index = 0; index < numEntries; index++) {
                PKIX_CHECK(PKIX_List_GetItem
                        (list, index, &current, plContext),
                        "PKIX_List_GetItem failed");

                if (current) {
                        PKIX_CHECK(PKIX_PL_Object_Equals
                                (object, current, &match, plContext),
                                "PKIX_PL_Object_Equals failed");

                        PKIX_DECREF(current);
                }

                if (match) {
                        PKIX_CHECK(PKIX_List_DeleteItem
                                (list, index, plContext),
                                "PKIX_List_DeleteItem failed");
                        break;
                }
        }

cleanup:

        PKIX_DECREF(current);
        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_MergeLists
 * DESCRIPTION:
 *
 *  Creates a new list consisting of the items from "firstList", followed by
 *  the items on "secondList", returns the new list at "pMergedList". If
 *  neither of the input lists contained any item, the result is an empty list.
 *  If an error occurrs, the result is NULL.
 *
 * PARAMETERS:
 *  "firstList"
 *      Address of list to be merged from. May be NULL or empty.
 *  "secondList"
 *      Address of list to be merged from. May be NULL or empty.
 *  "pMergedList"
 *      Address where returned object is stored.
 *  "plContext"
 *      platform-specific context pointer * THREAD SAFETY:
 *  Thread Safe (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds
 *  Returns a List Error if the functions fails in a non-fatal way
 *  Returns a Fatal Error if the function fails in an unrecoverable way
 */
PKIX_Error *
pkix_List_MergeLists(
        PKIX_List *firstList,
        PKIX_List *secondList,
        PKIX_List **pMergedList,
        void *plContext)
{
        PKIX_List *list = NULL;
        PKIX_PL_Object *item = NULL;
        PKIX_UInt32 numItems = 0;
        PKIX_UInt32 i;

        PKIX_ENTER(LIST, "pkix_List_MergeLists");
        PKIX_NULLCHECK_ONE(pMergedList);

        *pMergedList = NULL;

        PKIX_CHECK(PKIX_List_Create(&list, plContext),
                    "PKIX_List_Create failed");

        if (firstList != NULL) {

                PKIX_CHECK(PKIX_List_GetLength(firstList, &numItems, plContext),
                    "PKIX_List_GetLength failed");
        }

        for (i = 0; i < numItems; i++) {

                PKIX_CHECK(PKIX_List_GetItem(firstList, i, &item, plContext),
                        "PKIX_List_GetItem failed");

                PKIX_CHECK(PKIX_List_AppendItem(list, item, plContext),
                        "PKIX_List_AppendItem failed");

                PKIX_DECREF(item);
        }

        numItems = 0;
        if (secondList != NULL) {

                PKIX_CHECK(PKIX_List_GetLength
                        (secondList,
                        &numItems,
                        plContext),
                        "PKIX_List_GetLength failed");

        }

        for (i = 0; i < numItems; i++) {

                PKIX_CHECK(PKIX_List_GetItem
                        (secondList, i, &item, plContext),
                        "PKIX_List_GetItem failed");

                PKIX_CHECK(PKIX_List_AppendItem
                        (list, item, plContext), "PKIX_List_AppendItem failed");

                PKIX_DECREF(item);
        }

        *pMergedList = list;

cleanup:

        if (PKIX_ERROR_RECEIVED){
                PKIX_DECREF(list);
        }

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: pkix_List_AppendList
 * DESCRIPTION:
 *
 *  Append items on "fromList" to the "toList". Item reference count on
 *  "toList" is not incremented, but items appended from "fromList" is
 *  incremented.
 *
 * PARAMETERS:
 *  "toList"
 *      Address of list to be appended to. Must be non-NULL.
 *  "fromList"
 *      Address of list to be appended from. May be NULL or empty.
 *  "plContext"
 *      platform-specific context pointer
 * THREAD SAFETY:
 *  Thread Safe (see Thread Safety Definitions in Programmer's Guide)
 * RETURNS:
 *  Returns NULL if the function succeeds
 *  Returns a List Error if the functions fails in a non-fatal way
 *  Returns a Fatal Error if the function fails in an unrecoverable way
 */
PKIX_Error *
pkix_List_AppendList(
        PKIX_List *toList,
        PKIX_List *fromList,
        void *plContext)
{
        PKIX_PL_Object *item = NULL;
        PKIX_UInt32 numItems = 0;
        PKIX_UInt32 i;

        PKIX_ENTER(LIST, "pkix_List_AppendList");
        PKIX_NULLCHECK_ONE(toList);

        /* if fromList is NULL or is an empty list, no action */

        if (fromList == NULL) {
                goto cleanup;
        }

        PKIX_CHECK(PKIX_List_GetLength(fromList, &numItems, plContext),
                    "PKIX_List_GetLength failed");

        if (numItems == 0) {
                goto cleanup;
        }

        for (i = 0; i < numItems; i++) {

                PKIX_CHECK(PKIX_List_GetItem
                        (fromList, i, &item, plContext),
                        "PKIX_List_GetItem failed");

                PKIX_CHECK(PKIX_List_AppendItem(toList, item, plContext),
                            "PKIX_List_AppendItem failed");

                PKIX_DECREF(item);
        }

cleanup:

        PKIX_RETURN(LIST);
}

/* --Public-List-Functions--------------------------------------------- */

/*
 * FUNCTION: PKIX_List_Create (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_Create(
        PKIX_List **pList,
        void *plContext)
{
        PKIX_List *list = NULL;
        PKIX_Boolean isHeader = PKIX_TRUE;

        PKIX_ENTER(LIST, "PKIX_List_Create");
        PKIX_NULLCHECK_ONE(pList);

        PKIX_CHECK(pkix_List_Create_Internal(isHeader, &list, plContext),
                    "pkix_List_Create_Internal failed");

        *pList = list;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_SetImmutable (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_SetImmutable(
        PKIX_List *list,
        void *plContext)
{
        PKIX_ENTER(LIST, "PKIX_List_SetImmutable");
        PKIX_NULLCHECK_ONE(list);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        list->immutable = PKIX_TRUE;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_IsImmutable (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_IsImmutable(
        PKIX_List *list,
        PKIX_Boolean *pImmutable,
        void *plContext)
{
        PKIX_ENTER(LIST, "PKIX_List_IsImmutable");
        PKIX_NULLCHECK_TWO(list, pImmutable);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        *pImmutable = list->immutable;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_GetLength (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_GetLength(
        PKIX_List *list,
        PKIX_UInt32 *pLength,
        void *plContext)
{
        PKIX_ENTER(LIST, "PKIX_List_GetLength");
        PKIX_NULLCHECK_TWO(list, pLength);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        *pLength = list->length;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_IsEmpty (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_IsEmpty(
        PKIX_List *list,
        PKIX_Boolean *pEmpty,
        void *plContext)
{
        PKIX_UInt32 length;

        PKIX_ENTER(LIST, "PKIX_List_IsEmpty");
        PKIX_NULLCHECK_TWO(list, pEmpty);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        length = list->length;

        if (length == 0){
                *pEmpty = PKIX_TRUE;
        } else {
                *pEmpty = PKIX_FALSE;
        }

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_AppendItem (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_AppendItem(
        PKIX_List *list,
        PKIX_PL_Object *item,
        void *plContext)
{
        PKIX_List *lastElement = NULL;
        PKIX_UInt32 length, i;

        PKIX_ENTER(LIST, "PKIX_List_AppendItem");
        PKIX_NULLCHECK_ONE(list);

        if (list->immutable){
                PKIX_ERROR("Cannot call AppendItem on Immutable List");
        }

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        length = list->length;

        /* find last element of list and create new element there */

        lastElement = list;
        for (i = 0; i < length; i++){
                lastElement = lastElement->next;
        }

        PKIX_CHECK(pkix_List_Create_Internal
                    (PKIX_FALSE, &lastElement->next, plContext),
                    "pkix_List_Create_Internal failed");

        PKIX_INCREF(item);
        lastElement->next->item = item;

        PKIX_CHECK(PKIX_PL_Object_InvalidateCache
                    ((PKIX_PL_Object *)list, plContext),
                    "PKIX_PL_Object_InvalidateCache failed");

        list->length = list->length + 1;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_InsertItem (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_InsertItem(
        PKIX_List *list,
        PKIX_UInt32 index,
        PKIX_PL_Object *item,
        void *plContext)
{
        PKIX_List *element = NULL;
        PKIX_List *newElem = NULL;

        PKIX_ENTER(LIST, "PKIX_List_InsertItem");
        PKIX_NULLCHECK_ONE(list);


        if (list->immutable){
                PKIX_ERROR("Cannot call InsertItem on Immutable List");
        }

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        PKIX_CHECK(pkix_List_GetElement(list, index, &element, plContext),
                    "pkix_List_GetElement failed");

        /* Create a new list object */
        PKIX_CHECK(pkix_List_Create_Internal(PKIX_FALSE, &newElem, plContext),
                    "pkix_List_Create_Internal failed");

        /* Copy the old element's contents into the new element */
        newElem->item = element->item;

        /* Set the new element's next pointer to the old element's next */
        newElem->next = element->next;

        /* Set the old element's next pointer to the new element */
        element->next = newElem;

        PKIX_INCREF(item);
        element->item = item;

        PKIX_CHECK(PKIX_PL_Object_InvalidateCache
                    ((PKIX_PL_Object *)list, plContext),
                    "PKIX_PL_Object_InvalidateCache failed");

        list->length = list->length + 1;

cleanup:

        if (PKIX_ERROR_RECEIVED){
                PKIX_DECREF(newElem);
        }

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_GetItem (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_GetItem(
        PKIX_List *list,
        PKIX_UInt32 index,
        PKIX_PL_Object **pItem,
        void *plContext)
{
        PKIX_List *element = NULL;

        PKIX_ENTER(LIST, "PKIX_List_GetItem");
        PKIX_NULLCHECK_TWO(list, pItem);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        PKIX_CHECK(pkix_List_GetElement(list, index, &element, plContext),
                    "pkix_List_GetElement failed");

        PKIX_INCREF(element->item);
        *pItem = element->item;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_SetItem (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_SetItem(
        PKIX_List *list,
        PKIX_UInt32 index,
        PKIX_PL_Object *item,
        void *plContext)
{
        PKIX_List *element;

        PKIX_ENTER(LIST, "PKIX_List_SetItem");
        PKIX_NULLCHECK_ONE(list);

        if (list->immutable){
                PKIX_ERROR("Cannot call SetItem on Immutable List");
        }

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        PKIX_CHECK(pkix_List_GetElement(list, index, &element, plContext),
                    "pkix_List_GetElement failed");

        /* DecRef old contents */
        PKIX_DECREF(element->item);

        /* Set New Contents */
        PKIX_INCREF(item);
        element->item = item;

        PKIX_CHECK(PKIX_PL_Object_InvalidateCache
                    ((PKIX_PL_Object *)list, plContext),
                    "PKIX_PL_Object_InvalidateCache failed");

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_DeleteItem (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_DeleteItem(
        PKIX_List *list,
        PKIX_UInt32 index,
        void *plContext)
{
        PKIX_List *element = NULL;
        PKIX_List *prevElement = NULL;
        PKIX_List *nextElement = NULL;

        PKIX_ENTER(LIST, "PKIX_List_DeleteItem");
        PKIX_NULLCHECK_ONE(list);

        if (list->immutable){
                PKIX_ERROR("Cannot call DeleteItem on Immutable List");
        }

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        PKIX_CHECK(pkix_List_GetElement(list, index, &element, plContext),
                    "pkix_List_GetElement failed");

        /* DecRef old contents */
        PKIX_DECREF(element->item);

        nextElement = element->next;

        if (nextElement != NULL) {
                /* If the next element exists, splice it out. */

                /* Don't need to change ref counts for targets of next */
                element->item = nextElement->item;
                nextElement->item = NULL;

                /* Don't need to change ref counts for targets of next */
                element->next = nextElement->next;
                nextElement->next = NULL;

                PKIX_DECREF(nextElement);

        } else { /* The element is at the tail of the list */
                if (index != 0) {
                        PKIX_CHECK(pkix_List_GetElement
                                    (list, index-1, &prevElement, plContext),
                                    "pkix_List_GetElement failed");
                } else if (index == 0){ /* prevElement must be header */
                        prevElement = list;
                }
                prevElement->next = NULL;

                /* Delete the element */
                PKIX_DECREF(element);
        }

        PKIX_CHECK(PKIX_PL_Object_InvalidateCache
                    ((PKIX_PL_Object *)list, plContext),
                    "PKIX_PL_Object_InvalidateCache failed");

        list->length = list->length - 1;

cleanup:

        PKIX_RETURN(LIST);
}

/*
 * FUNCTION: PKIX_List_ReverseList (see comments in pkix_util.h)
 */
PKIX_Error *
PKIX_List_ReverseList(
        PKIX_List *list,
        PKIX_List **pReversedList,
        void *plContext)
{
        PKIX_List *reversedList = NULL;
        PKIX_PL_Object *item = NULL;
        PKIX_PL_Object *duplicateItem = NULL;
        PKIX_UInt32 length, i;

        PKIX_ENTER(LIST, "pkix_List_ReverseList");
        PKIX_NULLCHECK_TWO(list, pReversedList);

        if (!list->isHeader){
                PKIX_ERROR("Input List must be header");
        }

        length = list->length;

        /* Create a new list object */
        PKIX_CHECK(PKIX_List_Create(&reversedList, plContext),
                    "pkix_List_Create_Internal failed");

        /*
         * Starting with the last item and traversing backwards (from
         * the original list), append each item to the reversed list
         */

        for (i = 1; i <= length; i++){
                PKIX_CHECK(PKIX_List_GetItem
                            (list, (length - i), &item, plContext),
                            "PKIX_List_GetItem failed");

                PKIX_CHECK(PKIX_PL_Object_Duplicate
                            (item, &duplicateItem, plContext),
                            "pkix_List_Duplicate failed");

                PKIX_CHECK(PKIX_List_AppendItem
                            (reversedList, duplicateItem, plContext),
                            "PKIX_List_AppendItem failed");

                PKIX_DECREF(item);
                PKIX_DECREF(duplicateItem);
        }

        *pReversedList = reversedList;

cleanup:

        PKIX_DECREF(item);
        PKIX_DECREF(duplicateItem);

        if (PKIX_ERROR_RECEIVED){
                PKIX_DECREF(reversedList);
        }

        PKIX_RETURN(LIST);
}
