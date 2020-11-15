#ta = 0
#tb = pow2(16)
#tc = 0
#td = 100

#l = 1024
#s = tb/l

#x = 0:l:tb
#y = 0:1:s
#for i = 1:s
#y = (((td-tc)/(tb-ta))*(x(i)-ta) + (mod((td-tc),(tb-ta))*(x(i)-ta)))/((tb-ta)+tc)
#endfor
#plot(x,y)

#e=6
#for i=0:e
#ta = 0
#tb = 10
#tc = 0
#td = power(10,i)
#x = [0,1,2,3,4,5,6,7,8,9,10]
#y = (((td-tc)/(tb-ta))*(x-ta) + (mod((td-tc),(tb-ta))*(x-ta)))/((tb-ta)+tc)
#endfor
#plot(x,y)

#---
# octave script
# Description:
# this function map the grow value from input range to the equivalent value in the low output range
# Assume:
# tb-ta > td - tc
ta = 0
tb = 150
tc = 0
td = 100

l = 50

y = (((td-tc)/(tb-ta))*(l-ta) + (mod((td-tc),(tb-ta))*(l-ta)))/((tb-ta)+tc)
#printf("td-tc : %f\n", td - tc)
#printf("tb-ta : %f\n", tb - ta)
#printf("l-ta : %f\n", l - ta)
#printf("(td-tc)/(tb-ta) : %f\n", (td-tc)/(tb-ta)) # how much axis td diff is contained in tb
#printf("(td-tc)/(tb-ta)*(l-ta) : %f\n", (td-tc)/(tb-ta)*(l-ta)) # offset of l in td
#printf("(td-tc)mod(tb-ta) : %f\n", mod((td-tc),(tb-ta))) # 
#printf("(td-tc)mod(tb-ta)*(l-ta) : %f\n", mod((td-tc),(tb-ta))*(l-ta))
#printf("(td-tc)/(tb-ta)*(l-ta)+(td-tc)mod(tb-ta)*(l-ta) : %f\n", (td-tc)/(tb-ta)*(l-ta) + mod((td-tc),(tb-ta))*(l-ta))
#printf("(tb-ta)+tc : %f\n", (tb - ta) + tc)
#printf("(td-tc)/(tb-ta)*(l-ta)+(td-tc)mod(tb-ta)*(l-ta)/(tb-ta)+tc : %f\n", ((td-tc)/(tb-ta)*(l-ta) + mod((td-tc),(tb-ta))*(l-ta))/((tb - ta) + tc))

