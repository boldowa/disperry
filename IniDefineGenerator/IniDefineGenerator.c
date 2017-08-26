#include "bolib.h"
#include <assert.h>
#include <ctype.h>
#include <setjmp.h>

typedef struct _IniItem {
	char* type;
	char* name;
	char* initialValue;
}IniItem;

static void IniItemCleaner(void* data)
{
	IniItem *item = (IniItem*)data;
	free(item->type);
	free(item->name);
	free(item->initialValue);
}

static void SkipUntilAscii(const char* str, size_t* i, const size_t len)
{
	for(; (*i)<len; (*i)++)
	{
		if( !isascii(str[(*i)]) || IsSpace(str[(*i)]) )
		{
			break;
		}
	}
}

bool defsplit(const char* line, size_t* i, const size_t len, char** ptr)
{
	size_t st;
	size_t splen;
	char* split;

	st = (*i);
	SkipUntilAscii(line, i, len);
	if((st == (*i)) || (false == IsSpace(line[(*i)])))
	{
		return false;
	}

	splen = (*i)-st;
	split = malloc(sizeof(char)*(splen+1));
	assert(split);
	strncpy_s(split, splen+1, &line[st], splen);
	(*ptr) = split;
	return true;
}
bool GetDef(const char* line, void* item_v)
{
	jmp_buf e;
	char* type = NULL;
	char* name = NULL;
	char* initialValue = NULL;

	size_t len;
	size_t i;
	IniItem *item = (IniItem*)item_v;

	if(0 == setjmp(e))
	{
		i = 0;
		len = strlen(line);

		SkipSpaces(line, &i, len);
		if(false == defsplit(line, &i, len, &type)) longjmp(e, 1);
		SkipSpaces(line, &i, len);
		if(false == defsplit(line, &i, len, &name)) longjmp(e, 1);
		SkipSpaces(line, &i, len);
		if(false == defsplit(line, &i, len, &initialValue)) longjmp(e, 1);
	}
	else
	{
		free(type);
		free(name);
		free(initialValue);
		return false;
	}

	item->type = type;
	item->name = name;
	item->initialValue = initialValue;
	return true;
}

static void RemoveComment(char* line)
{
	char* s;
	int i;
	int len;
	s = strstr(line, "//");
	if(NULL != s)
	{
		*s = '\0';
	}

	len = (int)strlen(line);
	for(i = len-1; i>=0; i--)
	{
		if(false == IsSpace(line[i]))
		{
			break;
		}
		line[i] = '\0';
	}
}

static bool readInput(TextFile* in, List* items)
{
	const char* linebuf;
	char* buf;
	IniItem recv;
	IniItem* *item;

	FunexStruct fs[] = {
		{GetDef, &recv},
		{NULL, NULL}
	};

	linebuf = in->GetLine(in);
	while(NULL != linebuf)
	{
		buf = Str_copy(linebuf);
		RemoveComment(buf);
		if(0 == strlen(buf))
		{
			free(buf); buf = NULL;
			linebuf = in->GetLine(in);
			continue;
		}

		switch(FunexMatch(buf, fs))
		{
		case 0:
			item = malloc(sizeof(IniItem));
			assert(item);
			memcpy(item, &recv, sizeof(IniItem));
			items->enqueue(items, item);
			break;

		default:
			printf("%s: line %d: syntax error\n", in->super.path_get(&in->super), in->row_get(in));
			return false;
		}

		free(buf); buf= NULL;
		linebuf = in->GetLine(in);
	}

	return true;
}

static bool writeDefine(TextFile* out, List* items)
{
	Iterator* it;
	IniItem* item;
	char* prefix;

	// set class name
	prefix = Str_concat("C", out->super.name_get(&out->super));
	assert(prefix);

	// Header comment generate
	out->Printf(out, "/**\n");
	out->Printf(out, " * This code was generated by IniDefineGenerator\n");
	out->Printf(out, " * Copylight (C)boldowa\n");
	out->Printf(out, " */\n");
	out->Printf(out, "#pragma once\n\n");

	// Configs output
	out->Printf(out, "#define %s_DECLARE() \\\n", prefix);
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		out->Printf(out, "\t%s %s; \\\n", item->type, item->name);
	}
	out->Printf(out, "\n");

	// constructor output
	out->Printf(out, "#define %s_INIT() \\\n", prefix);
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		out->Printf(out, "\tthis->%s = %s; \\\n", item->name, item->initialValue);
	}
	out->Printf(out, "\n");

	// Get method output
	out->Printf(out, "#define %s_GET(STRUCT) \\\n", prefix);
	out->Printf(out, "\tSTRUCT cfg = { \\\n");
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		out->Printf(out, "\t\tthis->%s, \\\n", item->name);
	}
	out->Printf(out, "\t}; \\\n");
	out->Printf(out, "\treturn cfg; \\\n");
	out->Printf(out, "\n");

	// APPLY method output
	out->Printf(out, "#define %s_APPLY(INSTANCE) \\\n", prefix);
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		out->Printf(out, "\tthis->%s = INSTANCE.%s; \\\n", item->name, item->name);
	}
	out->Printf(out, "\n");

	// Keys output
	out->Printf(out, "#define %s_KEYDECS() \\\n", prefix);
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		item->name[0] = toupper(item->name[0]);
		out->Printf(out, "\tstatic const TCHAR* Key%s; \\\n", item->name);
	}
	out->Printf(out, "\n#define %s_KEYDEFS(CLASS) \\\n", prefix);
	for(it=items->begin(items); it!=NULL; it=it->next(it))
	{
		item = it->data(it);
		out->Printf(out, "\tconst TCHAR* CLASS::Key%s = _T(\"%s\"); \\\n", item->name, item->name);
	}
	out->Printf(out, "\n");

	// Load config output

	free(prefix);
	return true;
}

static bool Generate(const char* input)
{
	TextFile* in;
	TextFile* out;
	List* itemList;
	bool ret;

	itemList = new_List(NULL, IniItemCleaner);
	in = new_TextFile(input);
	{
		FilePath* fp = new_FilePath(input);
		fp->ext_set(fp, ".h");
		out = new_TextFile(fp->path_get(fp));
		delete_FilePath(&fp);
	}

	if((FileOpen_NoError != in->Open(in))
		|| (FileOpen_NoError != out->Open2(out, "w")))
	{
		printf("File open err\n");
		delete_TextFile(&in);
		delete_TextFile(&out);
		delete_List(&itemList);
		return false;
	}

	ret = readInput(in, itemList);
	if(ret) ret = writeDefine(out, itemList);

	delete_TextFile(&in);
	delete_TextFile(&out);
	delete_List(&itemList);

	return ret;
}

int main(int argc, char** argv)
{
	bool ret;

	if(argc != 2)
	{
		printf("Usage: %s <input>\n", argv[0]);
		return 0;
	}

	ret = Generate(argv[1]);
	if(!ret)
	{
		remove(argv[2]);
		return -1;
	}
	return 0;
}