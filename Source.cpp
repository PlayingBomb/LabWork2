#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int MAX_ELEMENTS = 12;

// ýëåìåíò
struct Element {
	float curr;
	float arg;
	int num;
};

// B > A !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void fast2sum(float a, float b, float* res, float* err) {
	*res = a + b;
	*err = a - (*res - b);
}

// ôóíêöèè ñëåä ýëåìåíòîâ
void next_elm_sin(Element* elm) {
	//printf("%f\n", elm->curr);
	elm->num++;
	elm->curr = elm->curr* -( (elm->arg*elm->arg) / ( (2.0f * elm->num) * (2.0f * elm->num + 1.0f) ) );
}
void next_elm_cos(Element* elm) {
	elm->num++;
	elm->curr = elm->curr * -( (elm->arg * elm->arg) / ( (2.0f * elm->num) * (2.0f * elm->num - 1.0f)));
}
void next_elm_exp(Element* elm) {
	elm->num++;
	elm->curr = elm->curr * (elm->arg / elm->num);
}
void next_elm_ln(Element* elm) {
	//printf("%f\n", elm->curr);
	elm->num++;
	elm->curr = elm->curr * -( elm->arg * (float)(elm->num) / (float)(elm->num+1) );
}
void next_elm_ln_any(Element* elm) {
	elm->num++;
	elm->curr = elm->curr * ( (elm->arg * elm->arg) * (2.0f * elm->num - 1.0f) / (2.0f * elm->num + 1.0f));
}

//ñ÷èòàëêà ( 1 - ïðÿìîé | 2 - îáðàòíûé | 3 - Êýõýí÷èê )
float count(float x, float curr_first, void (*formula)(Element* elm) , int mode) {
	float elements[MAX_ELEMENTS];
	Element elm = { curr_first, x, 0 };

	//printf("start count");
	
	//çàáèâàåì ìàññèâ ýëåìåíòàìè ðÿäà
	int k = 0;
	while (k < MAX_ELEMENTS) {
		elements[k] = elm.curr;
		formula(&elm);
		k++;
	}

	//printf("massive ready");

	//ñîáñíà ñ÷èòàëêà
	float res_sum = 0.0f;
	float keh_err = 0.0f;

	switch (mode)
	{
	case 1:
		for (int i = 0; i < MAX_ELEMENTS; i++)
			res_sum += elements[i];
		break;
	case 2:
		for (int i = MAX_ELEMENTS - 1; i >= 0;i--)
			res_sum += elements[i];
		break;
	case 3:
		float temp;
		float next_res, next_err;
		for (int i = 0; i < MAX_ELEMENTS;i++) {
			temp = elements[i] + keh_err;

			if (res_sum > temp)
				fast2sum(temp, res_sum, &next_res, &next_err);
			else
				fast2sum(res_sum, temp, &next_res, &next_err);

			res_sum = next_res;
			keh_err = next_err;
		}
		break;
	default:
		break;
	}
	return res_sum;
}

//íó è âûâîä
int main() {
	float arg;
	printf("Text down x:\n");
	scanf_s("%f", &arg);

	printf("\nsin(x)===============================\n");
	printf("Math.h: %.9f\n", sinf(arg));
	printf("\nStraight: %.9f\nError: %.9f\n", count(arg, arg, next_elm_sin, 1), count(arg, arg, next_elm_sin, 1) - sinf(arg));
	printf("\nBackwards: %.9f\nError: %.9f\n", count(arg, arg, next_elm_sin, 2), count(arg, arg, next_elm_sin, 2) - sinf(arg));
	printf("\nKehen: %.9f\nError: %.9f\n", count(arg, arg, next_elm_sin, 3), count(arg, arg, next_elm_sin, 3) - sinf(arg));

	printf("\ncos(x)===============================\n");
	printf("Math.h: %.9f\n", cosf(arg));
	printf("\nStraight: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_cos, 1), count(arg, 1.0f, next_elm_cos, 1) - cosf(arg));
	printf("\nBackwards: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_cos, 2), count(arg, 1.0f, next_elm_cos, 2) - cosf(arg));
	printf("\nKehen: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_cos, 3), count(arg, 1.0f, next_elm_cos, 3) - cosf(arg));

	printf("\nexp(x)===============================\n");
	printf("Math.h: %.9f\n", expf(arg));
	printf("\nStraight: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_exp, 1), count(arg, 1.0f, next_elm_exp, 1) - expf(arg));
	printf("\nBackwards: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_exp, 2), count(arg, 1.0f, next_elm_exp, 2) - expf(arg));
	printf("\nKehen: %.9f\nError: %.9f\n", count(arg, 1.0f, next_elm_exp, 3), count(arg, 1.0f, next_elm_exp, 3) - expf(arg));

	if ((arg > 0) && (arg < 2)) {
		printf("\nln(x) for x in (0; 2)==========================\n");
		printf("Math.h: %.9f\n", logf(arg));
		printf("\nStraight: %.9f\nError: %.9f\n", count(arg - 1, arg - 1, next_elm_ln, 1), count(arg - 1, arg - 1, next_elm_ln, 1) - logf(arg));
		printf("\nBackwards: %.9f\nError: %.9f\n", count(arg - 1, arg - 1, next_elm_ln, 2), count(arg - 1, arg - 1, next_elm_ln, 2) - logf(arg));
		printf("\nKehen: %.9f\nError: %.9f\n", count(arg - 1, arg - 1, next_elm_ln, 3), count(arg - 1, arg - 1, next_elm_ln, 3) - logf(arg));
	}
	else if (arg >= 2){
		float uni_arg = (arg - 1) / (arg + 1);
		printf("\nln(x) for any x========================\n");
		printf("Math.h: %.9f\n", logf(arg));
		printf("\nStraight: %.9f\nError: %.9f\n", 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 1), 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 1) - logf(arg));
		printf("\nBackwards: %.9f\nError: %.9f\n", 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 2), 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 2) - logf(arg));
		printf("\nKehen: %.9f\nError: %.9f\n", 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 3), 2.0f * count(uni_arg, uni_arg, next_elm_ln_any, 3) - logf(arg));

	}
}
