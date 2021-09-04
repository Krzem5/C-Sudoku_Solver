vt=[i//9 for i in range(0,81)]
ht=[(i%9)+9 for i in range(0,81)]
st=[i//27*3+(i%9)//3+18 for i in range(0,81)]



print("uint8_t _div_9_table[81]={"+",".join([str(e) for e in vt])+"};// i/9")
print("uint8_t _mod_9_plus_9_table[81]={"+",".join([str(e) for e in ht])+"};// (i%9)+9")
print("uint8_t _div_27_times_3_plus_mod_9_div_3_plus_18[81]={"+",".join([str(e) for e in st])+"};// i/27*3+(i%9)/3+18")
