rel_op	=	[rel_op:{=}]
rel_op	<>	[rel_op:{<}{>}]
rel_op	<	[rel_op:{<}]
rel_op	<=	[rel_op:{<}{=}]

term	7	[term:[factor:<number:7>]]
term	-123	[term:[factor:{-}<number:123>]]
term	8*-9	[term:[factor:<number:8>]{*}[factor:{-}<number:9>]]
term	-8*79	[term:[factor:{-}<number:8>]{*}[factor:<number:79>]]

arithm	8	[arithm:[term:[factor:<number:8>]]]
arithm	-8	[arithm:[term:[factor:{-}<number:8>]]]
arithm	8--8	[arithm:[term:[factor:<number:8>]]{-}[term:[factor:{-}<number:8>]]]
arithm	8-(-8)	[arithm:[term:[factor:<number:8>]]{-}[term:[factor:{(}[expr:[arithm:[term:[factor:{-}<number:8>]]]]{)}]]]

factor	-doo	[factor:{-}<ident:doo>]
factor	-do	BAD_EXPR
