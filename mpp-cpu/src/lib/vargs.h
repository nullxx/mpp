/* https://stackoverflow.com/a/66556553/10226903 */
#define EXPAND(x) x

#define _GLUE(X,Y) X##Y
#define GLUE(X,Y) _GLUE(X,Y)

#define _ARG_100(_,\
   _100,_99,_98,_97,_96,_95,_94,_93,_92,_91,_90,_89,_88,_87,_86,_85,_84,_83,_82,_81, \
   _80,_79,_78,_77,_76,_75,_74,_73,_72,_71,_70,_69,_68,_67,_66,_65,_64,_63,_62,_61, \
   _60,_59,_58,_57,_56,_55,_54,_53,_52,_51,_50,_49,_48,_47,_46,_45,_44,_43,_42,_41, \
   _40,_39,_38,_37,_36,_35,_34,_33,_32,_31,_30,_29,_28,_27,_26,_25,_24,_23,_22,_21, \
   _20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,X_,...) X_
#define HAS_COMMA(...) EXPAND(_ARG_100(__VA_ARGS__, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

#define _TRIGGER_PARENTHESIS_(...) ,
#define _PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _IS_EMPTY_CASE_0001 ,
#define _IS_EMPTY(_0, _1, _2, _3) HAS_COMMA(_PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define IS_EMPTY(...)  \
   _IS_EMPTY(                                                               \
      /* test if there is just one argument, eventually an empty    \
         one */                                                     \
      HAS_COMMA(__VA_ARGS__),                                       \
      /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
         adds a comma */                                            \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
      /* test if the argument together with a parenthesis           \
         adds a comma */                                            \
      HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
      /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
         parenthesis adds a comma */                                \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
   )

#define _VAR_COUNT_EMPTY_1(...) 0
#define _VAR_COUNT_EMPTY_0(...) EXPAND(_ARG_100(__VA_ARGS__, \
   100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81, \
   80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61, \
   60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41, \
   40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21, \
   20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1))
#define VAR_COUNT(...) GLUE(_VAR_COUNT_EMPTY_, IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)