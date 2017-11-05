#ifndef PARSE_H
#define PARSE_H
int start(FILE*, struct element*);

int args(FILE*, struct element*);
int rel_op(FILE*, struct element*);
int expr(FILE*, struct element*);
int arithm(FILE*, struct element*);
int term(FILE*, struct element*);
int factor(FILE*, struct element*);

#endif /* PARSE_H */