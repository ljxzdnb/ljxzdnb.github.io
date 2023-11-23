
#define tim 2
#define div 4
#define odd 8
#define add 16
#define mil 32
#define and 64

#define OPS_SELECT(ops,a,b) ((ops)&&tim?((ops)&&div?((ops)&&odd?((ops)&&add?((ops)&&mil?((ops)&&and?(0):(a&b)):(a-b)):(a+b)):(a%b)):(a/b)):(a*b))
#define OPS_SELECTS(ops,a,b) (ops&tim? (a*b):(ops&div? (a/b):(ops&odd? (a%b):(ops&add? (a+b):(ops&mil? (a-b):(ops&and? (a&b):(0)))))))


int main(){
	int a = 4;
	int b[] = {8,13,2,14};
	int ops = 16;
	for (int i = 0;i < 4;i++){
	a = OPS_SELECTS(ops,a,b[i]);
	printf("a:%d\n",a);
	}
	printf("a:%d\n",a);
//	printf("%d %d %d",ops & tim,a/b,OPS_SELECTS(ops,a,b));
}

