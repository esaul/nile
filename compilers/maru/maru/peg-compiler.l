(define-structure <peg-compiler> (grammar program))

(define-function peg-compiler (grammar)
  (with-instance-accessors <peg-compiler>
    (let ((self (new <peg-compiler>)))
      (set self.grammar grammar)
      (set self.program (array))
      self)))

(define-selector $compile)
(define-selector $emit)
(define-selector $gen)
(define-selector $declare)
(define-selector $define)
(define-selector $find-variables)
(define-selector $match-both)
(define-selector $match-rule)
(define-selector $match-first)
(define-selector $match-object)
(define-selector $match-class)
(define-selector $match-list)
(define-selector $assign-result)
(define-selector $match-zero-more)
(define-selector $match-one-more)
(define-selector $result-variable)
(define-selector $result-list-variable)
(define-selector $match-zero-one)
(define-selector $result-list-symbol)
(define-selector $result-list)
(define-selector $make-symbol)
(define-selector $make-string)
(define-selector $make-number)
(define-selector $make-span)
(define-selector $match-string)
(define-selector $match-any)
(define-selector $peek-not)
(define-selector $result-list-variable-splicing)
(define-selector $result-object)
(define-selector $peek-for)

(define-method $compile <peg-compiler> ()
  ($emit self (list 'define-class
		    (string->symbol (<grammar>-name self.grammar))
		    (string->symbol (<grammar>-base self.grammar))
		    (<grammar>-fields self.grammar)))
  (with-map $declare self (<grammar>-rules self.grammar))
  (with-map $define  self (<grammar>-rules self.grammar))
  (array->list self.program))

(define-method $emit <peg-compiler> (arg)
  (array-append self.program arg))

(define-method $declare <peg-compiler> (rule)
  ($emit self (list 'define-selector (concat-symbol '$ (car rule)))))

(define-method $define <peg-compiler> (rule)
  (let ((vars ($find-variables self (cadr rule) ())))
    ($emit self (list 'define-method (concat-symbol '$ (car rule)) (string->symbol (<grammar>-name self.grammar)) ()
		      (list 'let ($find-variables self (cadr rule) ())
			    ($gen self (cadr rule)))))))

(define-method $find-variables <peg-compiler> (tree vars)
  (and (pair? tree)
       (if (= (car tree) 'assign-result)
	   (or (assq (cadr tree) vars)
	       (set vars (cons (cons (cadr tree)) vars)))
	 (let ((e1 (cadr tree))
	       (e2 (caddr tree)))
	   (set vars ($find-variables self e1 vars))
	   (set vars ($find-variables self e2 vars)))))
  vars)

(define-method $gen <peg-compiler> (tree)
  ;;(println "- " tree)
  (let ((op (car tree)))
    (cond
      ((= op 'match-both)			($match-both			self tree))
      ((= op 'match-rule)			($match-rule			self tree))
      ((= op 'match-first)			($match-first			self tree))
      ((= op 'match-object)			($match-object			self tree))
      ((= op 'match-class)			($match-class			self tree))
      ((= op 'match-list)			($match-list			self tree))
      ((= op 'assign-result)			($assign-result			self tree))
      ((= op 'match-zero-more)			($match-zero-more		self tree))
      ((= op 'match-one-more)			($match-one-more		self tree))
      ((= op 'result-variable)			($result-variable		self tree))
      ((= op 'result-list-variable)		($result-list-variable		self tree))
      ((= op 'match-zero-one)			($match-zero-one		self tree))
      ((= op 'result-list-symbol)		($result-list-symbol		self tree))
      ((= op 'result-list)			($result-list			self tree))
      ((= op 'make-symbol)			($make-symbol			self tree))
      ((= op 'make-string)			($make-string			self tree))
      ((= op 'make-number)			($make-number			self tree))
      ((= op 'make-span)			($make-span			self tree))
      ((= op 'match-string)			($match-string			self tree))
      ((= op 'match-any)			($match-any			self tree))
      ((= op 'peek-not)				($peek-not			self tree))
      ((= op 'result-list-variable-splicing)	($result-list-variable-splicing self tree))
      ((= op 'result-object)			($result-object			self tree))
      ((= op 'peek-for)				($peek-for			self tree))
      (else					(error "peg-compile does not understand "op)))))

(define-function peg-compiler-gen-sequence (self type tree)
  (if (= type (car tree))
      (cons ($gen self (cadr tree))
	    (peg-compiler-gen-sequence self type (caddr tree)))
    (cons ($gen self tree))))

(define-method $match-both <peg-compiler> (tree)
  `(let ((pos (<parser-stream>-position self.source)))
     (or (and ,@(peg-compiler-gen-sequence self 'match-both tree))
	 (parser-stream-fail self.source pos))))

(define-method $match-rule <peg-compiler> (tree)
  (list (concat-symbol '$ (cadr tree)) 'self))

(define-method $match-first <peg-compiler> (tree)
  `(or ,@(peg-compiler-gen-sequence self 'match-first tree)))

(define-method $match-object <peg-compiler> (tree)
  `(and (= ',(cadr tree) (parser-stream-peek self.source))
	(set self.result (parser-stream-next self.source))))

(define-method $match-class <peg-compiler> (tree)
  `(set self.result (parser-stream-match-class self.source ',(cadr tree))))

(define-method $match-list <peg-compiler> (tree)
  `(and (pair? (parser-stream-peek self.source))
	(let ((pos (<parser-stream>-position self.source))
	      (src self.source))
	  (set self.source (parser-stream (list-stream (parser-stream-next src))))
	  (let ((ok (and ,($gen self (cadr tree)))))
	    (set self.source src)
	    (or ok (parser-stream-fail self.source pos))))))

(define-method $assign-result <peg-compiler> (tree)
  `(let () (set ,(cadr tree) self.result) 't))

(define-method $match-zero-more <peg-compiler> (tree)
  `(let ((_list_ (group)))
     (while ,($gen self (cadr tree)) (group-append _list_ self.result))
     (set self.result (group->list! _list_))
     't))

(define-method $match-one-more <peg-compiler> (tree)
  `(let ((_list_ (group)))
     (while ,($gen self (cadr tree)) (group-append _list_ self.result))
     (and (not (group-empty? _list_))
	  (let ()
	    (set self.result (group->list! _list_))
	    't))))

(define-method $match-zero-one <peg-compiler> (tree)
  `(let ((_list_ (group)))
     (and ,($gen self (cadr tree)) (group-append _list_ self.result))
     (set self.result (group->list! _list_))
     't))

(define-method $result-variable		<peg-compiler> (tree)	`(let () (set self.result ,(cadr tree)) 't))

(define-method $result-list-variable	<peg-compiler> (tree)	`(group-append _list_ ,(cadr tree)))
(define-method $result-list-symbol	<peg-compiler> (tree)	`(group-append _list_ ',(cadr tree)))

(define-method $result-list <peg-compiler> (tree)
  `(let ((_list_ (group)))
     ,@(with-map $gen self (cdr tree))
     (set self.result (group->list! _list_))
     't))

(define-method $make-symbol <peg-compiler> (tree)
  `(and ,($gen self (cadr tree))
	(set self.result (string->symbol (list->string self.result)))))

(define-method $make-string <peg-compiler> (tree)
  `(and ,($gen self (cadr tree))
	(set self.result (list->string self.result))))

(define-method $make-number <peg-compiler> (tree)
  `(and ,($gen self (cadr tree))
	(set self.result (string->number (list->string self.result)))))

(define-method $make-span <peg-compiler> (tree)
  `(let ((pos (<parser-stream>-position self.source)))
     (and ,($gen self (cadr tree))
	  (let ()
	    (set self.result (list-from-to pos (<parser-stream>-position self.source)))
	    't))))

(define-method $match-string <peg-compiler> (tree)
  (list 'set 'self.result (list 'parser-stream-match-string 'self.source (cadr tree))))

(define-method $match-any <peg-compiler> (tree)
  '(set self.result (parser-stream-next self.source)))

(define-method $peek-not <peg-compiler> (tree)
  `(let ((pos (<parser-stream>-position self.source)))
     (if ,($gen self (cadr tree))
	 (parser-stream-fail self.source pos)
       't)))

(define-method $result-list-variable-splicing <peg-compiler> (tree)
  (list 'group-append-list '_list_ (cadr tree)))

(define-method $result-object <peg-compiler> (tree)
  `(let () (set self.result ,(cadr tree)) 't))

(define-method $peek-for <peg-compiler> (tree)
  `(let ((pos (<parser-stream>-position self.source)))
     (and ,($gen self (cadr tree))
	  (set (<parser-stream>-position self.source) pos))))
