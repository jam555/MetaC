typedef struct master_context
{
	stackpair *stkp;
	
	int argn;
	char **args;
	
} master_context;


	/* optn and opts are the strings that are recognized as */
	/*  command-line arguments. They are allowed to be followed */
	/*  by an equals sign on the actual command line, but shouldn't */
	/*  directly contain one themselves. The index of the first */
	/*  recognized option will be recorded in the matches instance */
	/*  that corresponds to the currently searched args instance, or */
	/*  else will include -1. The position of the first encountered */
	/*  equals sign will be recorded in eqpoints, or else -1. */
	/* args, matches, and eqpoints must all have argn members; optl */
	/*  and opts must have optn members; the members of args and */
	/*  opts must be null terminated strings; the members of optl */
	/*  must be the lengths of the corresponding members of opts. */
int arg_parser
(
	int argn, const char **args,
	int *matches, int *eqpoints,
	
	size_t optn, size_t *optl, const char **opts
);



#define common_entrance_func_CLANG ( 0 )
#define common_entrance_func_CppLANG ( 1 )
#define common_entrance_func_FORTRANLANG ( 2 )
#define common_entrance_func_METACLANG ( 3 )
#define common_entrance_func_OBJCLANG ( 4 )
#define common_entrance_func_TEXT ( 5 )

const char *commandline_options[] =
{
	"-lC",
	"-lC++",
	"-lFortran",
	"-lMetaC",
	"-lObjC",
	"-lText"
};
const size_t commandline_options_lengths[] =
{
	sizeof( commandline_options[ common_entrance_func_CLANG ] ),
	sizeof( commandline_options[ common_entrance_func_CppLANG ] ),
	sizeof( commandline_options[ common_entrance_func_FORTRANLANG ] ),
	sizeof( commandline_options[ common_entrance_func_METACLANG ] ),
	sizeof( commandline_options[ common_entrance_func_OBJCLANG ] ),
	sizeof( commandline_options[ common_entrance_func_TEXT ] )
};
const int commandline_options_count =
	sizeof( commandline_options ) / sizeof( commandline_options[ 0 ] );
