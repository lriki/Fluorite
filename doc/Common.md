
���
--------------------
### ������

### �\����� (SyntaxParser)
AST �����B
���̎��_�ł͂܂� Symbol �͍��Ȃ��B

Roslyn �Ȃ� CSharp.Syntax.MethodDeclarationSyntax �Ƃ��������B

### �Ӗ���� (SemanticAnalyzer)
�����܂ł��Ă悤�₭ Symbol ������Ă����B



���[�X�P�[�X
--------------------
### �t�@�C����o�^���ĉ�͂���
```
AnalyzerContext* ac = AnalyzerContext::Create();
ac->RegisterSourceFile("test.cpp");
ac->AnalyzeAll();
```

### ����s���܂�ł���֐���`��T�� (Symbol ����)
```
SymbolDatabase* db = ac->GetSymbolDatabase();
Enumerable<Symbol*> sources = db->GetSourceFileSymbols();
Symbol* source = sources.First([](Symbol* s){ s->GetName() == "C:\\test.cpp"; });
Enumerable<Reference*> refs = source.GetReferences();
Enumerable<Reference*> funcDecls = refs.Where([](Reference* r){ r->GetKind() == ReferenceDecl::FunctionDeclaration; });
Reference* funcDeclRef  = refs.First([](Reference* r){ r->GetSourceRange().ContainsLineNumber(100); });
Symbol* func = funcDeclRef->GetSymbol();
```



Token
--------------------

### �|�C���^�R�s�[���A���ԃR�s�[��
�g�[�N���̔z��� Array<Token*> �Ƃ��邩�AArray<Token> �Ƃ��邩�A�Ƃ������B

�ȑO�� Token �͎��ԁA�R�[�h�� String �͎Q�Ƃ������B
�����A���ꂾ�ƃI���W�i���̃\�[�X�R�[�h�o�b�t�@�͂����Ɛ����Ă��Ȃ���΂Ȃ炸�A�Ǘ��I�ɂ߂�ǂ������B

����� clang ��������Q�l�ɂ��Ă݂����̂����A�\����̓G���W���Ƃ��Ă� Token 1�ɂ��Ȃ肽������̏�����������K�v������B
���̂����R�s�[�R�X�g���傫���Ȃ肻���B
���Ȃ݂� und �� Token �Q�Ƃł���BCppCheck ���B

Fluorite �Ƃ��Ă͈ȉ��̂悤�ɂ��Ă݂�B
- Token �͎Q��
- �R�[�h�� String �͎��� (Lex ��A�I���W�i���̃o�b�t�@�͎̂ĂĂ��ǂ��悤�ɂ���)

Reference
--------------------
ASTNode(Syntax) �Ƃ͕ʕ��B����炾������ CallBy ��\���ł��Ȃ��B



Kind
--------------------
�����̌���𓯎��ɉ�͂���Ƃ��A���Ă����ʂ�����g���܂킷�A�̓i�V�B
�Ⴆ�΁AC++ �� FuncDecl �� C# �� MethodDecl �͕ʂɂ���B�ق��ɂ� FuncPtr �Ƃ� Delegate �Ƃ��B



���\�[�X�Ǘ��E�V���A���C�Y
--------------------
�V���A���C�Y�̓C�e���[�V�����������I�ɍs�����߂ɕK�v�B
�܂�Ainclude �t�@�C�����݂���Ȃ� �� include �p�X�ݒ肵�čĉ�͂Ƃ��A
��̓G���[�̂Ƃ��A�t�@�C��������Ƃ��������čĉ�͂Ƃ��̃L���b�V���B


�����I�ȍĉ��
--------------------
�܂��A��͂������t�@�C���Ɋ܂܂�Ă���S�Ă� Reference ���폜����B
�폜������A���̎Q�Ɛ�� Entity �̎Q�ƃJ�E���g��1���炷�B
0�ɂȂ����� Entity ���폜����B



���̑�����
--------------------
�\����͂��I������i�K�ł̓}�N���W�J�Ȃǂ͂��Ȃ��B
���ł��W�J�ł���悤�ɏ������낦�Ă��������B



