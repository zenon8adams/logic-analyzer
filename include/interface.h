#ifndef INTERFACE_H
#define INTERFACE_H
#include "config.hpp"
#include <stddef.h>
#include <stdio.h>

typedef struct LogicalExpCollection LogicalExpCollection;
typedef struct LogicModel LogicModel;
typedef struct TableModel TableModel;
typedef struct BinaryCounter BinaryCounter;
typedef struct PostfixGenerator PostfixGenerator;
typedef int bool;

extern LogicModel *makeLogicModel(wchar_t **array, size_t size);
extern void model(LogicModel *lmodel, LogicalExpCollection *obj_coll);
MAYBE_UNUSED extern size_t numberOfVariable(LogicModel *lexp);
extern void destroyLogicModelObject(LogicModel *lmodel);

extern TableModel *makeTableModel(LogicModel *logic_model);
extern void addOutputChannel(TableModel *table_model, FILE *fp);
extern void setDisplayModeAscending(TableModel *tmodel, bool asc);
extern void display(TableModel *table_model);
extern void destroyTableModelObject(TableModel *table_model);

MAYBE_UNUSED extern BinaryCounter *createCounter_s(const char *ini);
extern BinaryCounter *createCounter(size_t size, char bit);
extern BinaryCounter *Inc(BinaryCounter *counter);
extern BinaryCounter *Dec(BinaryCounter *counter);
MAYBE_UNUSED extern void
destroyCounterObject(BinaryCounter *obj);

extern PostfixGenerator *makePostfixGenerator(const wchar_t *input);
extern wchar_t **postfixForm(PostfixGenerator *gen, size_t *len);
extern void destroyPostfixObject(PostfixGenerator *obj);

extern FILE *tty0();
extern wchar_t *toWString(const char *str);
extern void freeArray(wchar_t **array, size_t size);
extern void freeString(wchar_t *c);

#endif
