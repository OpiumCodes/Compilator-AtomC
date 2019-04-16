
int sum()
{
	int	 i,v[5],s;
	s=0;
	for(i=0;i<5;i=i+1){
		v[i]=i;
		s=s+2;
		}
	return s;
}

void main()
{
	int		i,s;
	for(i=0;i<10000;i=i+1)
	s=sum();
	put_i(s);
}

