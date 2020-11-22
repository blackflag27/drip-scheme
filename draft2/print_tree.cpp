string print_tree(Object obj)
{
	// Only outputs pairs, for everything else it doesn't return
	// anything.
	
	if (obj.type != Type::Pair)
		return "";
	
	

