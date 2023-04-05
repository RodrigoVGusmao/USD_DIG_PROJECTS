#ifndef CTRYCATCH_H_INCLUDED
#define CTRYCATCH_H_INCLUDED

//this "try" "catch" block have many differences from c++ try catch blocks,
//first you cannot create e throw label without a catch pair, code executed
//in catch and try blocks will fall through

#define TRY const char* __ctc_err_msg = NULL;
#define FALL_THROUGH_CATCH(lbError, message) goto ctc_goto_jump_over_##lbError;\
lbError: __ctc_err_msg = message; ctc_goto_jump_over_##lbError:
#define CATCH(lbError, message) if(0){ lbError: __ctc_err_msg = message; \
goto __ctc_goto_exclusive_block_##lbError;} if(0) \
__ctc_goto_exclusive_block_##lbError:
#define WHAT __ctc_err_msg

#ifdef __GNUC__
#define THROW_ON_FAIL(bCondition, lbError) if(__builtin_expect(bCondition, 0)) \
goto lbError;
#else
#define THROW_ON_FAIL(bCondition, lbError) if(bCondition) goto lbError;
#endif

#endif // CTRYCATCH_H_INCLUDED
