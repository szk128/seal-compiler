
//**************************************************************
//
// Code generator SKELETON
//
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <vector>

using namespace std;

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3};

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

//////////////////////////////////////////////////////////////////
//
//
//    Helper Functions
//  
//
//////////////////////////////////////////////////////////////////
typedef std::map<Symbol, int> VariableTable;
typedef std::map<Symbol, VariableTable> FunctionTable;
FunctionTable functab;

typedef std::map<Symbol, Symbol> GlobalTable;
GlobalTable globalTab;
// you can add any helper functions here


//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}


//*********************************************************
//
// Define method for code generation
//
//
//*********************************************************

void Program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  cgen_helper(decls,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_mov(const char *source, const char *dest, ostream& s)
{
  s << MOV << source << COMMA << dest << endl;
}

static void emit_rmmov(const char *source_reg, int offset, const char *base_reg, ostream& s)
{
  s << MOV << source_reg << COMMA << offset << "(" << base_reg << ")"
      << endl;
}

static void emit_mrmov(const char *base_reg, int offset, const char *dest_reg, ostream& s)
{
  s << MOV << offset << "(" << base_reg << ")" << COMMA << dest_reg  
      << endl;
}

static void emit_irmov(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_immov(const char *immidiate, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << "(" << offset << ")" << base_reg  
      << endl;
}

static void emit_add(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADD << source_reg << COMMA << dest_reg << endl;
}

static void emit_sub(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUB << source_reg << COMMA << dest_reg << endl;
}

static void emit_mul(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MUL << source_reg << COMMA << dest_reg << endl;
}

static void emit_div(const char *dest_reg, ostream& s)
{
  s << DIV << dest_reg << endl;
}

static void emit_cqto(ostream &s)
{
  s << CQTO << endl;
}

static void emit_neg(const char *dest_reg, ostream& s)
{
  s << NEG << dest_reg << endl;
}

static void emit_and(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << AND << source_reg << COMMA << dest_reg << endl;
}

static void emit_or(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << OR << source_reg << COMMA << dest_reg << endl;
}

static void emit_xor(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XOR << source_reg << COMMA << dest_reg << endl;
}

static void emit_not(const char *dest_reg, ostream& s)
{
  s << NOT << " " << dest_reg << endl;
}

static void emit_movsd(const char *source, const char *dest, ostream& s)
{
  s << MOVSD << source << COMMA << dest << endl;
}

static void emit_movaps(const char *source, const char *dest, ostream& s)
{
  s << MOVAPS << source << COMMA << dest << endl;
}

static void emit_addsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADDSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_subsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUBSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_mulsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MULSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_divsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << DIVSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_cmp(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << CMP << source_reg << COMMA << dest_reg << endl;
}

static void emit_test(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << TEST << source_reg << COMMA << dest_reg << endl;
}

static void emit_ucompisd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << UCOMPISD << source_reg << COMMA << dest_reg << endl;
}

static void emit_xorpd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XORPD << source_reg << COMMA << dest_reg << endl;
}
static void emit_jmp(const char *dest, ostream& s)
{
  s << JMP << " " << dest << endl;
}

static void emit_jl(const char *dest, ostream& s)
{
  s << JL << " " << dest << endl;
}

static void emit_jle(const char *dest, ostream& s)
{
  s << JLE << " " << dest << endl;
}

static void emit_je(const char *dest, ostream& s)
{
  s << JE << " " << dest << endl;
}

static void emit_jne(const char *dest, ostream& s)
{
  s << JNE << " " << dest << endl;
}

static void emit_jg(const char *dest, ostream& s)
{
  s << JG << " " << dest << endl;
}

static void emit_jge(const char *dest, ostream& s)
{
  s << JGE << " " << dest << endl;
}

static void emit_jb(const char *dest, ostream& s)
{
  s << JB << " " << dest << endl;
}

static void emit_jbe(const char *dest, ostream& s)
{
  s << JBE << " " << dest << endl;
}

static void emit_ja(const char *dest, ostream& s)
{
  s << JA << " " << dest << endl;
}

static void emit_jae(const char *dest, ostream& s)
{
  s << JAE << " " << dest << endl;
}

static void emit_jp(const char *dest, ostream& s)
{
  s << JP << " " << dest << endl;
}

static void emit_jz(const char *dest, ostream& s)
{
  s << JZ << " " << dest << endl;
}

static void emit_jnz(const char *dest, ostream& s)
{
  s << JNZ << " " << dest << endl;
}

static void emit_call(const char *dest, ostream& s)
{
  s << CALL << " " << dest << endl;
}

static void emit_ret(ostream& s)
{
  s << RET << endl;
}

static void emit_push(const char *reg, ostream& s)
{
  s << PUSH << " " << reg << endl;
}

static void emit_pop(const char *reg, ostream& s)
{
  s << POP << " " << reg << endl;
}

static void emit_leave(ostream& s)
{
  s << LEAVE << endl;
}

static void emit_position(const char *p, ostream& s)
{
  s << p << ":" << endl;
}

static void emit_float_to_int(const char *float_mmx, const char *int_reg, ostream& s)
{
  s << CVTTSD2SIQ << float_mmx << COMMA << int_reg << endl;
}

static void emit_int_to_float(const char *int_reg, const char *float_mmx, ostream& s)
{
  s << CVTSI2SDQ << int_reg << COMMA << float_mmx << endl;
}
///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Seal has four kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// If you like, you can add any ***Entry::code_def() and ***Entry::code_ref()
// functions to help.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << "$" << STRINGCONST_PREFIX << index;
}

//
// Emit code for a constant String.
//

void StringEntry::code_def(ostream& s)
{
  s << STRINGCONST_PREFIX << index << ":" << endl;
  s  << STRINGTAG ; emit_string_constant(s,str);                                                // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s);
}

// the following 2 functions are useless, please DO NOT care about them
void FloatEntry::code_ref(ostream &s)
{
  s << FLOATTAG << index;
}

void IntEntry::code_def(ostream &s)
{
  s << GLOBAL;
}

//***************************************************
//
//  Emit global var and functions.
//
//***************************************************

static void emit_global_int(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  INTTAG << 0 << endl;
}

static void emit_global_float(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl <<
  FLOATTAG << 0 << endl <<
  FLOATTAG << 0 << endl;
}

static void emit_global_bool(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  BOOLTAG << 0 << endl;
}

void code_global_data(Decls decls, ostream &str) {
  int count = 0;
  for (int i=decls->first(); decls->more(i); i=decls->next(i)) {
    if (!decls->nth(i)->isCallDecl()) {
      count ++;
      if (count == 1) str<<DATA<<endl;
      Symbol name = decls->nth(i)->getName();
      Symbol type = decls->nth(i)->getType();
      globalTab[name] = type;
      if (type == Int) {
        emit_global_int(name, str);
      } else if (type == Bool) {
        emit_global_bool(name, str);
      } else if (type == Float) {
        emit_global_float(name, str);
      }
    }
  }
}

void code_calls(Decls decls, ostream &str) {
  str<<SECTION<<RODATA<<endl;
  stringtable.code_string_table(str);
  str<<TEXT<<endl;
  for (int i=decls->first(); decls->more(i); i=decls->next(i)) {
    if (decls->nth(i)->isCallDecl()) {
      decls->nth(i)->code(str);
    }
  }
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************



//********************************************************
//
// Cgen helper helps to initialize and call code() function.
// You can do any initializing operations here
//
//********************************************************

void cgen_helper(Decls decls, ostream& s)
{

  code(decls, s);
}


void code(Decls decls, ostream& s)
{
  if (cgen_debug) cout << "Coding global data" << endl;
  code_global_data(decls, s);

  if (cgen_debug) cout << "Coding calls" << endl;
  code_calls(decls, s);
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `seal-decl.h', `seal-expr.h' and `seal-stmt.h'
//   Sample code for constant integers, strings, and booleans are provided.
//   
//*****************************************************************

void CallDecl_class::code(ostream &s) {
  Symbol name = this->getName();
  Symbol type = this->getType();
  Variables vars = this->getVariables();
  StmtBlock stmtblock = this->getBody();
  
  VariableTable vartab;

  s<<GLOBAL<<name<<endl<<
  SYMBOL_TYPE<<name<<COMMA<<FUNCTION<<endl;

  s<<name<<":"<<endl;
  emit_push(RBP, s);
  emit_mov(RSP, RBP, s);
  emit_push(RBX, s);
  emit_push(R10, s);
  emit_push(R11, s);
  emit_push(R12, s);
  emit_push(R13, s);
  emit_push(R14, s);
  emit_push(R15, s);  

  int order = 1;
  int num = 1;
  for (int i=vars->first(); vars->more(i); i=vars->next(i)) {
    Symbol name = vars->nth(i)->getName();
    Symbol type = vars->nth(i)->getType();
    vartab[name] = num;
    num ++;

    switch (order) {
      case 1:
        emit_sub("$8", RBP, s);
        emit_mov(RDI, RBP, s);
        order ++;
        break;
      case 2:
        emit_sub("$8", RBP, s);
        emit_mov(RSI, RBP, s);
        order ++;
        break;
      case 3:
        emit_sub("$8", RBP, s);
        emit_mov(RDX, RBP, s);
        order ++;
        break;
      case 4:
        emit_sub("$8", RBP, s);
        emit_mov(RCX, RBP, s);
        order ++;
        break;
      case 5:
        emit_sub("$8", RBP, s);
        emit_mov(R8, RBP, s);
        order ++;
        break;
      case 6:
        emit_sub("$8", RBP, s);
        emit_mov(R9, RBP, s);
        order ++;
        break;
    }
  }

  // variableDecls
  VariableDecls varDecls = stmtblock->getVariableDecls();
  for (int i=varDecls->first(); varDecls->more(i); i=varDecls->next(i)) {
    Symbol name = varDecls->nth(i)->getName();
    Symbol type = varDecls->nth(i)->getType();
    vartab[name] = num;
    num ++;
    emit_sub("$8", RSP, s);
  }

  functab[name] = vartab;
  
  stmtblock->code(s);
  s<<SIZE<<name<<", "<<".-"<<name<<endl;
}

void StmtBlock_class::code(ostream &s){
  Stmts stmts = this->getStmts();
  for (int i=0; stmts->more(i); i=stmts->next(i)) {
    stmts->nth(i)->code(s);
  }
}

void IfStmt_class::code(ostream &s) {
  condition->code(s);
  emit_mov(RBP, RAX, s);
  emit_test(RAX, RAX, s);
  emit_jz("POSIF2", s);
  thenexpr->code(s);
  emit_jmp("POS ifEND", s);
  s<<"POSIF2"<<":"<<endl;
  elseexpr->code(s);
  s<<"POS ifEND"<<endl;
}

void WhileStmt_class::code(ostream &s) {
  s<<"POS condition"<<endl;
  condition->code(s);
  emit_mov(RBP, RAX, s);
  emit_test(RAX, RAX, s);
  emit_jz("POS whileEND", s);
  body->code(s);
  emit_jmp("POS condition", s);
  s<<"POS whileEND"<<endl;
}

void ForStmt_class::code(ostream &s) {
  initexpr->code(s);
  s<<"POS condition:"<<endl;
  condition->code(s);
  emit_mov(RBP, RAX, s);
  emit_test(RAX, RAX, s);
  emit_jz("POS ForEND", s);
  body->code(s);
  s<<"POS loopact:"<<endl;
  loopact->code(s);
  emit_jmp("POS condition", s);
  s<<"POS forEND"<<endl;
}

void ReturnStmt_class::code(ostream &s) {
  emit_pop(R15, s);
  emit_pop(R14, s);
  emit_pop(R13, s);
  emit_pop(R12, s);
  emit_pop(R11, s);
  emit_pop(R10, s);
  emit_pop(RBX, s);

  s<<LEAVE<<endl
  <<RET<<endl;
}

void ContinueStmt_class::code(ostream &s) {
 
}

void BreakStmt_class::code(ostream &s) {
}

void Call_class::code(ostream &s) {
  
  //
  /*
   if function name is printf

    // please set %eax to the number of Float parameters, num.
    //  把%eax赋值为Float类型的参数个数, num
    emit_sub("$8", RSP, s);
    emit_irmovl(num, EAX, s);
    emit_call("printf", s);
      
    return;
  }
  */
  //
}

void Actual_class::code(ostream &s) {
  
}

void Assign_class::code(ostream &s) {
  Symbol lv = lvalue;
  emit_sub("$8", RSP, s);

  // global or local
  bool flag = false;
  if (globalTab[lv] != NULL) flag = true;
  
  Expr rv = value;
  rv->code(s);
  
  emit_mov(RBP, RAX, s);

  if (flag == false) {
    emit_mov(RAX, RBP, s);
  } else {
    emit_mov(RAX, RIP, s);
  }
  
}

void Add_class::code(ostream &s) {
  e1->code(s);
  e2->code(s);
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RBX, s);
  emit_mov(RBP, R10, s);
  emit_add(RBX, R10, s);
  emit_mov(R10, RBP, s);
}

void Minus_class::code(ostream &s) {
  e1->code(s);
  e2->code(s);
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RBX, s);
  emit_mov(RBP, R10, s);
  emit_sub(RBX, R10, s);
  emit_mov(R10, RBP, s);
}

void Multi_class::code(ostream &s) {
  e1->code(s);
  e2->code(s);
  emit_sub("$8", RSP, s);  
  emit_mov(RBP, RBX, s);
  emit_mov(RBP, R10, s);
  emit_mul(RBX, R10, s);
  emit_mov(R10, RBP, s);
}

void Divide_class::code(ostream &s) {
  e1->code(s);
  e2->code(s);
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_cqto(s);
  emit_mov(RBP, RBX, s);
  emit_div(RBX, s);
  emit_mov(RAX, RBP, s);
}

void Mod_class::code(ostream &s) {
  e1->code(s);
  e2->code(s);
  emit_sub("$8", RSP, s);  
  emit_mov(RBP, RAX, s);
  emit_cqto(s);
  emit_mov(RBP, RBX, s);
  emit_div(RBX, s);
  emit_mov(RDX, RBP, s);
}

void Neg_class::code(ostream &s) {
  e1->code(s);
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_neg(RAX, s);
  emit_mov(RAX, RBP, s);
}

void Lt_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_jl("POSLT0", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POSLT1", s);
  emit_position("POSLT0", s);
  emit_mov("$1", RAX, s);
  emit_position("POSLT1", s);
  emit_mov(RAX, RBP, s);
}

void Le_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_jle("POS", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POS", s);
  emit_position("POS", s);
  emit_mov("$1", RAX, s);
  emit_position("POS", s);
  emit_mov(RAX, RBP, s);
}

void Equ_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_je("POS", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POS", s);
  emit_position("POS", s);
  emit_mov("$1", RAX, s);
  emit_position("POS", s);
  emit_mov(RAX, RBP, s);
}

void Neq_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_jne("POS", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POS", s);
  emit_position("POS", s);
  emit_mov("$1", RAX, s);
  emit_position("POS", s);
  emit_mov(RAX, RBP, s);
}

void Ge_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_jge("POS", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POS", s);
  emit_position("POS", s);
  emit_mov("$1", RAX, s);
  emit_position("POS", s);
  emit_mov(RAX, RBP, s);
}

void Gt_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_cmp(RDX, RAX, s);
  emit_jg("POS", s);
  emit_mov("$0", RAX, s);
  emit_jmp("POS", s);
  emit_position("POS", s);
  emit_mov("$1", RAX, s);
  emit_position("POS", s);
  emit_mov(RAX, RBP, s);
}

void And_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_and(RAX, RDX, s);
  emit_mov(RDX, RBP, s);
}

void Or_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_or(RAX, RDX, s);
  emit_mov(RDX, RBP, s);
}

void Xor_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_xor(RAX, RDX, s);
  emit_mov(RDX, RBP, s);
}

void Not_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov("$0x0000000000000001", RDX, s);
  emit_xor(RDX, RAX, s);
  emit_mov(RAX, RBP, s);
}

void Bitnot_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_not(RAX, s);
  emit_mov(RAX, RBP, s);
}

void Bitand_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_and(RAX, RDX, s);
  emit_mov(RDX, RBP, s);
}

void Bitor_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov(RBP, RAX, s);
  emit_mov(RBP, RDX, s);
  emit_or(RAX, RDX, s);
  emit_mov(RDX, RBP, s);
}

void Const_int_class::code(ostream &s) {
  s << MOV << "$" << value << COMMA << RAX << endl
    << MOV << RAX << COMMA << RBP << endl;
}

void Const_string_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  emit_mov("LC", RAX, s);
  emit_mov(RAX, RBP, s);
}

void Const_float_class::code(ostream &s) {
 
}

void Const_bool_class::code(ostream &s) {
  emit_sub("$8", RSP, s);

  if (value == 1) {
    emit_mov("$1", RAX, s);
  } else {
    emit_mov("$0", RAX, s);
  }

  emit_mov(RAX, RBP, s);
}

void Object_class::code(ostream &s) {
 
}

void No_expr_class::code(ostream &s) {

}