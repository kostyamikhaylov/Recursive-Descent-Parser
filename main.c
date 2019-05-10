#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define STR_LEN 40


//
//	RULES:
//	
//	N::=['0'-'9']+
//	P::={'('E')'}|N
//	X::=P{'^'X}*
//	T::=X{['*''/']X}*
//	E::=T{['+''-']T}*
//	A::=E|{Id'='E}
//	I::='if''('A')'O
//	O::=[AIC]';'
//	C::='{'O+'}'
//	G::=C'\0'
//
//


const char *p = NULL;
char vars[STR_LEN];
int vars_pos;

int GetG (const char *str);
int GetC (void);
int GetO (void);
int GetI (void);
int GetA (void);
int GetE (void);
int GetT (void);
int GetX (void);
int GetP (void);
int GetN (void);

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf (stderr, "Usage: <prog_name>  <expression>\n");
		exit (EXIT_FAILURE);
	}

	printf ("Result: %d\n", GetG (argv[1]));
	printf ("Result: str: %s\n", vars);

	return 0;
}

int GetG (const char *str)
{
	assert (str);
	p = str;

	int res = GetC ();

	if (*p != '\0')
	{
		fprintf (stderr, "GetG: Error on character: '%s'\n", p);
		exit (EXIT_FAILURE);
	}
	p++;

	return res;
}

int GetC (void)
{
	int val = 0;
	
	if (*p != '{')
	{
		fprintf (stderr, "GetC: '{' expected\n");
		return -1;
	}
	p++;

	while (!strncmp (p, "if", 2) || *p == '{' || ('A' <= *p && *p <= 'z'))
	{
		GetO ();
	}
	
	if (*p != '}')
	{
		fprintf (stderr, "GetC: '}' expected\n");
		return -1;
	}
	p++;
	
	return val;
}

int GetO (void)
{
	int val = 0;
	if (!strncmp (p, "if", 2))
	{
		val = GetI ();
	}
	else if (*p == '{')
	{
		val = GetC ();
	}
	else if ('A' <= *p && *p <= 'z')
	{
		val = GetA ();
		if (*p != ';')
		{
			fprintf (stderr, "GetO: ';' expected\n");
			return -1;
		}
		p++;
	}
	else
	{
		fprintf (stderr, "GetO: A, I or C expected\n");
		return -1;
	}
	
	return val;
}

int GetI ()
{
	int val = 0;
	if (strncmp (p, "if", 2))
	{
		fprintf (stderr, "GetI: 'if' expected\n");
		return 0;
	}
	p += 2;
	
	if (*p != '(')
	{
		fprintf (stderr, "GetI: '(' expected\n");
		return -1;
	}
	p++;
	val = GetA ();
	if (*p != ')')
	{
		fprintf (stderr, "GetI: ')' expected\n");
		return -1;
	}
	p++;

	if (val)
		GetO ();
	else
	{
		int save_vars = vars_pos;
		GetO ();
		memset (vars + save_vars, '\0', vars_pos - save_vars);
		vars_pos = save_vars;
	}

	return val;
}

int GetA (void)
{
	int val = 0;
	int count = 0;
	char buf[STR_LEN] = {};

	if ('A' <= *p && *p <= 'z')
	{
		while (('A' <= *p && *p <= 'z') || *p == '_')
		{
			buf[count++] = *p;
			p++;
			if (count == STR_LEN - 1)
			{
				fprintf (stderr, "GetA: Variable name is too long\n");
				return -1;
			}
		}
		if (*p != '=')
			return 0;
		buf[count++] = *p;
		p++;

		val = GetE ();

		snprintf (buf + count, STR_LEN - count - 1, "%d", val);

		strncpy (vars + vars_pos, buf, STR_LEN - vars_pos - 1);

		vars_pos += strlen (buf);

		return val;
	}

	return GetE ();
}

int GetE (void)
{
	int val = 0;
	val = GetT ();

	while (*p == '+' || *p == '-')
	{
		const char *save_p = p;
		p++;
		int val2 = GetT ();

		if (*save_p == '+')
			val += val2;
		else
			val -= val2;
	}
	return val;
}

int GetT (void)
{
	int val = 0;
	val = GetX ();

	while (*p == '*' || *p == '/')
	{
		const char *save_p = p;
		p++;
		int val2 = GetX ();

		if (*save_p == '*')
			val *= val2;
		else
		{
			if (val2 == 0)
			{
				fprintf (stderr, "GetT: Zero division on character: '%s'\n", save_p);
				exit (EXIT_FAILURE);		
			}
			val /= val2;
		}
	}
	return val;
}

int GetX (void)
{
	int val = 0;
	val = GetP ();
	if (*p == '^')
	{
		p++;
		int val2 = GetX ();
		return pow (val, val2);
	}
	return val;
}

int GetP (void)
{
	int val = 0;
	if (*p == '(')
	{
		p++;
		val = GetE ();
		if (*p != ')')
		{
			fprintf (stderr, "GetP: No closing bracket; instead: '%s'\n", p);
			exit (EXIT_FAILURE);
		}
		p++;
		return val;
	}
	else
		return GetN ();
}

int GetN (void)
{
	int val = 0;
	const char *save_p = p;

	while ('0' <= *p && *p <= '9')
	{
		val = val * 10 + *p - '0';
		p++;
	}

	if (p == save_p)
	{
		fprintf (stderr, "GetN: Error on character: '%s'\n", p);
		exit (EXIT_FAILURE);
	}

	return val;
}

