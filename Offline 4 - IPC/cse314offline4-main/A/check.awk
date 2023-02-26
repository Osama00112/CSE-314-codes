BEGIN {
    for (i = 0; i < MAX; i++) {
	prod[i] = 0
	cons[i] = 0
    }
}

{
    if ($1 == "Produced" && $2 < MAX) {
	prod[$2]++;
    }
    else if ($1 == "Consumed" && $2 < MAX) {
	cons[$2]++;
    }
    
}

END {
	count=0;
	m=0;
	n=0;
	for (i = 0; i < MAX; i++) {
		if (prod[i] == 1) {
			m++;
		}
		if(cons[i] == 1) {
			n++;
		}
	}
	printf("Produced %d  Consumed  %d\n",m,n);

	for (i = 0; i < MAX; i++) {
		if (prod[i] == 1 && cons[i] == 1) {
			count++;
		}
		else {
#			printf("%d\t%d\t%d\n", i, prod[i], cons[i]);	
		}
	}
	if (count == MAX) {
		printf("OK. All test cases passed!\n");
	}
	else {
		printf("Please check your code. %d/%d test cases failed!\n", MAX - count, MAX);
	}
	while (1) {
		
	}
}
