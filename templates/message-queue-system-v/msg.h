#define AUFTRAGS_MSGTYP 1

enum {sinus, cosinus, tangens};
char *s[] = {"Sinus", "Cosinus", "Tangens"};

struct auftrag {
	long erg_msgtyp;
	int funktno;
	double argument;
};

struct ergebnis {
	int funktno;
	double argument;
	double resultat;
};


struct auftrag_msg {
	long mtype;
	struct auftrag auftrag;
};

struct ergebnis_msg {
	long mtype;
	struct ergebnis ergebnis;
};


