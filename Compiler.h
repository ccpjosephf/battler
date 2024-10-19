#pragma once

#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include <bitset>
#include <unordered_map>
#include <tuple>

#include "vm/game.h"

using std::vector;
using std::string;
using std::bitset;
using std::unordered_map;
using std::tuple;

namespace Battler {


	enum class OpcodeType
	{
		// Block headers
		GAME_BLK_HEADER = 0,
		CARD_BLK_HEADER,
		SETUP_BLK_HEADER,
		PHASE_BLK_HEADER,
		TURN_BLK_HEADER,
		IF_BLK_HEADER,
		FOREACHPLAYER_BLK_HEADER,
		FOREACHPLAYER_BLK_END,
		BLK_END,

		// Math & Logic
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		PLAYERS_L_VALUE,
		L_VALUE_DOT_SEPERATED_REF_CHAIN,
		PLAYERS_R_VALUE,
		L_VALUE,
		R_VALUE_REF,
		R_VALUE_DOT_SEPERATED_REF_CHAIN,
		DOT_SEPERATED_REF_CHAIN_END,
		R_VALUE,
		COMPARE,

		// Stack operations
		STACK_SOURCE_RANDOM_CARD_TYPE,
		STACK_SOURCE_TOP,
		STACK_SOURCE_BOTTOM,
		STACK_SOURCE_CHOOSE,
		STACK_DEST_TOP,
		STACK_DEST_BOTTOM,

		// other declarations
		ATTR_DECL,
		ATTR_ASSIGN,
		ATTR_DATA_TYPE,
		ATTR_DATA,
		DO_DECL,
		WINNER_DECL,

		// just in case we need it
		NO_OP,
	};

	enum class PROC_MODE
	{
		GAME,
		CARD,
		PHASE,
		SETUP,
		TURN,
		FOREACH,
		IF,
	};

#define TYPE_CODE_T uint16_t
#define DATA_IX_T uint32_t
#define OPCODE_CONV_T uint64_t

#define TYPE_CODE_T_MASK uint16_t(0xFF)

	// data type codes
	const TYPE_CODE_T STRING_TC = 0x01;
	const TYPE_CODE_T INT_TC = 0x02;
	const TYPE_CODE_T BOOL_TC = 0x03;
	const TYPE_CODE_T FLOAT_TC = 0x04;
	const TYPE_CODE_T VISIBLE_STACK_TC = 0x05;
	const TYPE_CODE_T HIDDEN_STACK_TC = 0x06;
	const TYPE_CODE_T PRIVATE_STACK_TC = 0x07;
	const TYPE_CODE_T CARD_TC = 0x08;
	const TYPE_CODE_T STACK_REF = 0x09;
	const TYPE_CODE_T PLAYER_REF_TC = 0x10;

	typedef void(stack_move_callback_fun)(
		int from,
		int to,
		bool from_top,
		bool to_top,
		const int* cardIds,
		int nCards,
		void* data
		);

	class Opcode
	{
	public:
		Opcode() : data(0), blk_size(0) {};
		OpcodeType type;
		int blk_size;
		uint64_t data;
	};

	class Program
	{
	public:
		Program() : m_depth(0), m_current_opcode_index(0), m_stack_move_callback(nullptr) {};
		void Compile(vector<string>);
		int Run(bool load = false);
		int RunSetup();
		int RunTurn();
		vector<AttrCont>& locale_stack();

		void SetStackMoveCallbackFun(stack_move_callback_fun* fun, void* data);

		vector<Opcode> opcodes();
		Game game();

	private:
		//compiled data
		vector<Opcode> m_opcodes;
		vector<Token> m_tokens;

		Expression m_rootExpression;

		vector<string> m_strings;
		vector<int> m_ints;
		vector<bool> m_bools;

		int m_setup_index;
		int m_turn_index;
		int m_depth;
		unordered_map<string, int> m_phase_indexes;

		//runtime data
		Game m_game;
		int m_current_opcode_index;
		vector<AttrCont> m_locale_stack;
		vector<PROC_MODE> m_proc_mode_stack;
		vector<string> m_block_name_stack;

		stack_move_callback_fun* m_stack_move_callback;
		void* m_stack_callback_data;

		void factor_expression(Expression);
		void compile_name(vector<Token>, bool lvalue);

		int run(Opcode code, bool load = false);

		static AttributeType s_type_code_to_attribute_type(TYPE_CODE_T);

		void ignore_block();
		void compile_expression(Expression);

		//copied from run.h
		AttrCont* GetObjectAttrContPtrFromIdentifier(vector<string>::iterator namesBegin, vector<string>::iterator namesEnd);
		AttrCont* GetGlobalObjectAttrContPtr(AttrCont& cont, string name);
		int resolve_number_expression();
		bool resolve_bool_expression();
		string resolve_string_expression();
		float resolve_float_expression();
		Attr resolve_expression_to_attr();

		void read_name(vector<string>& names, OpcodeType nameType);
		Attr* get_attr_ptr(vector<string>& names);
		Attr get_attr_rvalue(vector<string>& names);
		string get_card_parent_name(string nameSequence);
		string get_card_name(string nameSequence);
		Stack* get_stack_ptr(vector<string>& names);
		bool compare_attrs(Attr a, Attr b);
		Attr subtract_attrs(Attr a, Attr b);
		Attr add_attrs(Attr a, Attr b);
		Attr multiply_attrs(Attr a, Attr b);
		Attr divide_attrs(Attr a, Attr b);

		void call_stack_move_callback(int from, int to, bool fromTop, bool toTop, const int* cardIds, int nCards);
	};

	class CompileError {
	public:
		Token t;
		string reason;
		CompileError(string reason, Token t) : reason{ reason }, t{ t } {}
	};

	class VMError {
	public:
		string reason;
		VMError(string reason) : reason{ reason } {}
	};

}

#endif // !COMPILER_H