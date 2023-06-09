#include "../inc/intermediate.h"
#include "../inc/cwd.h"

int reg = 1;
int lab = 1;

type *newTypeCreate() {
	type *newType = malloc(sizeof(type));
	newType->str = malloc(sizeof(char) * 100);
	return newType;
}

type *newTypePrint(FILE *fp) {
	type *newType = newTypeCreate();

	newType->reg = 1;
	sprintf(newType->str, "r%d", reg++);
	newType->num = 0;
	fprintf(fp, "%s = ", newType->str);

	return newType;
}

label *newLabPrint() {
	label *newLab = malloc(sizeof(label));
	newLab->name = malloc(sizeof(char) * 100);

	sprintf(newLab->name, "l%d", lab++);
	return newLab;
}

type *newType_printCode(type *first, type *second, char *op, FILE *fp) {
	type *newType, *temp;

	if (!strcmp(op, "=2")) {
		if (second->reg)
			return second;

		temp = newTypePrint(fp);
		if (second->str)
			fprintf(fp, "%s", second->str);
		else
			fprintf(fp, "%d", second->num);
		fprintf(fp, "\n");
		return temp;
	} else if (!strcmp(op, "=")) {
		newType = newTypeCreate();
		newType->num = 0;
		newType->reg = 0;
		newType->str = strdup(((struct symref *)(((struct symasgn *)first)->s))->s->name);
		fprintf(fp, "%s = %s\n", newType->str, newType_printCode(first, second, "=2", fp)->str);
		return newType;
	}

	newType = newTypePrint(fp);

	if (first->reg || first->str)
		fprintf(fp, "%s", first->str);
	else
		fprintf(fp, "%d", first->num);
	fprintf(fp, " %s ", op);
	if (second->reg || second->str)
		fprintf(fp, "%s", second->str);
	else
		fprintf(fp, "%d", second->num);
	fprintf(fp, "\n");
	return newType;
}

type *intermediateCode(struct ast *a, FILE *fp) {
	type *left, *right, *newType = newTypeCreate(), *cond;
	newType->num = 0;
	newType->reg = 0;
	newType->str = 0;

	switch (a->nodetype) {
	case 'K':
		newType->num = ((struct numval *)a)->number;
		newType->reg = 0;
		newType->str = 0;
		return newType;

	case 'N':
		newType->num = 0;
		newType->reg = 0;
		newType->str = strdup(((struct symref *)a)->s->name);
		return newType;

	case '=':
		right = intermediateCode(((struct symasgn *)a)->v, fp);
		return newType_printCode((type *)a, right, "=", fp);

	case '+':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "+", fp);
	case '-':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "-", fp);
	case '*':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "*", fp);
	case '/':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "/", fp);
	case '1':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, ">", fp);
	case '2':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "<", fp);
	case '3':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "<>", fp);
	case '4':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "==", fp);
	case '5':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, ">=", fp);
	case '6':
		left = intermediateCode(a->l, fp);
		right = intermediateCode(a->r, fp);
		return newType_printCode(left, right, "<=", fp);

	case 'I':
		cond = newTypeCreate();
		label *lab1 = newLabPrint(), *lab2 = newLabPrint();

		if (((struct astIf *)a)->cond)
			cond = intermediateCode(((struct astIf *)a)->cond, fp);

		fprintf(fp, "if ");
		if (cond->reg || cond->str)
			fprintf(fp, "%s", cond->str);
		else
			fprintf(fp, "%d", cond->num);

		fprintf(fp, " goto %s\n", lab1->name);
		if (((struct astIf *)a)->el) {
			intermediateCode(((struct astIf *)a)->el, fp);
		}
		fprintf(fp, "goto %s\n%s:\n", lab2->name, lab1->name);
		if (((struct astIf *)a)->tl) {
			intermediateCode(((struct astIf *)a)->tl, fp);
		}
		fprintf(fp, "%s\n", lab2->name);

		return newType;

	case 'L':
		intermediateCode(a->l, fp);
		intermediateCode(a->r, fp);
		return newType;


	case 'W':
		cond = newTypeCreate();
		lab1 = newLabPrint();
		lab2 = newLabPrint();
		label *lab3 = newLabPrint();

		fprintf(fp, "%s:\n", lab1->name);
		if (((struct astWh *)a)->cond)
			cond = intermediateCode(((struct astWh *)a)->cond, fp);

		fprintf(fp, "if ");
		if (cond->reg || cond->str)
			fprintf(fp, "%s", cond->str);
		else
			fprintf(fp, "%d", cond->num);

		fprintf(fp, " goto %s\ngoto %s\n%s:\n", lab2->name, lab3->name, lab2->name);
		if (((struct astWh *)a)->tl) {
			intermediateCode(((struct astIf *)a)->tl, fp);
		}

		fprintf(fp, "goto %s\n", lab1->name);
		fprintf(fp, "%s:\n", lab3->name);
		return newType;

	case 'F':
		cond = newTypeCreate();
		lab1 = newLabPrint();
		lab2 = newLabPrint();
		lab3 = newLabPrint();

		fprintf(fp, "%s:\n", lab1->name);
		cond = intermediateCode(((struct astFor *)a)->exp1, fp);

		fprintf(fp, "if ");
		if (cond->reg)
			fprintf(fp, "%s", cond->str);
		else
			fprintf(fp, "%s", ((struct symref *)(((struct astFor *)a)->name))->s->name);

		fprintf(fp, " < ");
		cond = intermediateCode(((struct astFor *)a)->exp2, fp);

		if (cond->reg || cond->str)
			fprintf(fp, "%s", cond->str);
		else
			fprintf(fp, "%d", cond->num);

		fprintf(fp, " goto %s\ngoto %s\n%s:\n", lab2->name, lab3->name, lab2->name);
		if (((struct astFor *)a)->tl) {
			intermediateCode(((struct astFor *)a)->tl, fp);
		}

		newType = newTypePrint(fp);
		fprintf(fp, "%s + 1\n", ((struct symref *)(((struct astFor *)a)->name))->s->name);
		fprintf(fp, "%s = %s\n", ((struct symref *)(((struct astFor *)a)->name))->s->name, newType->str);

		fprintf(fp, "goto %s\n", lab1->name);
		fprintf(fp, "%s:\n", lab3->name);

		newType->num = 0;
		newType->reg = 0;
		newType->str = 0;
		return newType;
	}
	return newType;
}

void intermediateCodeHandle(struct ast *a) {
	char completePath[500];
	getCompletePath(completePath, "/output/intermediate.txt");

	FILE *fp = fopen(completePath, "a+");

	if (fp == NULL) {
		printf("Error opening the file.\n");
		return;
	}

	intermediateCode(a, fp);
	fprintf(fp, "\n\n");
	fclose(fp);
}
