(define (domain dice)
(:requirements :strips)
(:predicates
 (adjUL ?x ?y)
 (adjUR ?x ?y)
 (adjLL ?x ?y)
 (adjLR ?x ?y)
 (adjD ?x ?y)
 (actual ?x))

(:action spinUL
:parameters (?current ?next)
:precondition (and (adjUL ?current ?next) (actual ?current))
:effect (and (actual ?next) (not (actual ?current)))
)
(:action spinUR
:parameters (?current ?next)
:precondition (and (adjUR ?current ?next) (actual ?current))
:effect (and (actual ?next) (not (actual ?current)))
)
(:action spinLL
:parameters (?current ?next)
:precondition (and (adjLL ?current ?next) (actual ?current))
:effect (and (actual ?next) (not (actual ?current)))
)
(:action spinLR
:parameters (?current ?next)
:precondition (and (adjLR ?current ?next) (actual ?current))
:effect (and (actual ?next) (not (actual ?current)))
)
(:action spinD
:parameters (?current ?next)
:precondition (and (adjD ?current ?next) (actual ?current))
:effect (and (actual ?next) (not (actual ?current)))
)
)
