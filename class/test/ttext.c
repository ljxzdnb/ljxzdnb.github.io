struct test_t{
	int num;
	struct test_t* self;
};

int main(){
	struct test_t test = {2,&test};
	printf("%d\n",test.self->num);
	printf("%p\n",&test);
	printf("%p\n",test.self);
}
