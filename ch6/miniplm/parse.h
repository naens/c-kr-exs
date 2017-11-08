#ifndef PARSE_H
#define PARSE_H

int start(FILE*, struct element*);
int module(FILE *file, struct element *element);
int declaration(FILE *file, struct element *element);
int decl_statement(FILE *file, struct element *element);
int decl_element(FILE *file, struct element *element);
int initial(FILE *file, struct element *element);
int procedure(FILE *file, struct element *element);
int proc_statement(FILE *file, struct element *element);
int params(FILE *file, struct element *element);
int unit(FILE *file, struct element *element);
int cond(FILE *file, struct element *element);
int do_block(FILE *file, struct element *element);
int do_while(FILE *file, struct element *element);
int do_iter(FILE *file, struct element *element);
int block_end(FILE *file, struct element *element);
int statement(FILE *file, struct element *element);
int args(FILE*, struct element*);
int rel_op(FILE*, struct element*);
int expr(FILE*, struct element*);
int arithm(FILE*, struct element*);
int term(FILE*, struct element*);
int factor(FILE*, struct element*);

#endif /* PARSE_H */