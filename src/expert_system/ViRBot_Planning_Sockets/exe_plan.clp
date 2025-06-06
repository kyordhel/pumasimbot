;************************************************
;*                                              *
;*      exe_plan.clp	                        *
;*                                              *
;*                                              *
;*                                              *
;*                      J.Savage, UNAM          *
;*                                              *
;*                      1/5/20                  *
;*                                              *
;************************************************



(defrule send-num_plans
        (finish-planner ?name ?num_pln)
        =>
        (printout t "send num-plans " ?num_pln  crlf)
        (assert (send-ROS ACT-PLN num_plans ?num_pln 1.0 4))
	;(assert (exe-plan ?name ?num_pln))
	(assert (send-plan ?name ?num_pln))
)

(defrule attempt
	(declare (salience -1000))
	?f1 <- (attempt (move ?object) (room ?room)(on ?place&:(neq ?place freespace))(status nil))
	(item (type Objects) (name ?object)(room ?room))
        =>
        (modify ?f1 (status accomplished))
)


(defrule attempt-freespace
        (declare (salience -2000))
        ?f1 <- (attempt (move ?object) (room ?room)(on freespace)(status nil))
        =>
        (modify ?f1 (status accomplished))
)


(defrule erase-attempt 
	?f1 <- (attempt (move ?object) (room ?room)(number ?num)(status accomplished))
        =>
        (retract ?f1)
        (printout t "Plan number " ?num " move " ?object " to " ?room " accomplished" crlf)
)


(defrule send-plan
	(declare (salience 100))
	;(step ?num)
	?f <- (send-plan ?name ?num)
        (plan (name ?name) (number ?num)(actions $?arguments)(status inactive))
        =>
	(retract ?f)
        (printout t "send plan " $?arguments  crlf)
        (assert (send-ROS ACT-PLN ?num $?arguments ))
	(assert (send-plan ?name (- ?num 1)))
)




(defrule exe-plan
        (exe-plan ?name ?num_pln)
        ?f <- (plan (name ?name) (number ?num&:(neq num 0))(status inactive))
        (not (plan (name ?name) (number ?num1&:( < ?num1 ?num))(status ?status&:(or (eq ?status active) (eq ?status inactive  )))) )
        =>
        (modify ?f (status active))
)



(defrule exe-plan-find-object
        (plan (name ?name) (number ?num-pln)(status active)(actions find-object ?obj)(duration ?t))
 	;?f1 <- (item (name ?obj)(status ?x&:(neq ?x found)))
 	?f1 <- (item (name ?obj)(pose ?x ?y ?z))
        =>
	(bind ?command (str-cat "" ?obj " " ?x " " ?y " " ?z""))
        (assert (send-ROS ACT-PLN find_object ?command ?t 4))
)




(defrule exe-plan-found-object
        ?f <-  (answer ?sender command find_object ?block1 ?x ?y ?z ?arm 1)
 	?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions find-object ?object))
	;?f3 <- (Arm (name ?arm))
        =>
        (retract ?f)
        (modify ?f2 (status accomplished))
        ;(modify ?f3 (status verify))
        ;(modify ?f1 (pose ?x ?y ?z) (status found));;;; modified for verify arm task		
        (modify ?f1 (pose ?x ?y ?z))		
)



;(item (type Robot) (name robot))
;(executed plan 1 goto 1)
;(plan (number 1) (actions goto corridor deposit))
(defrule executed-plan-goto
	?f0 <- (executed plan ?num goto 1)
        ?f1 <- (plan (number ?num)(actions goto ?room ?zone))
	?f2 <- (item (type Robot) (name robot))
        =>
	(retract ?f0 ?f1)
	;(printout t "delete goto plan " ?num " " ?room  crlf)
	(modify ?f2 (room ?room)(zone ?zone))
)

;(plan (name cubes) (number 78) (actions drop book) )
;(executed plan 78 drop 1)
;(item (type Objects) (name book)) 
(defrule executed-plan-drop
	?f0 <- (executed plan ?num drop 1)
	?f3 <- (update object ?object room ?room)
        ?f1 <- (plan (number ?num)(actions drop ?object))
	?f2 <- (item (type Objects) (name ?object)) 
        =>
	(retract ?f0 ?f1 ?f3)
	;(printout t "delete drop plan " ?num " " ?object  crlf)
	(modify ?f2 (room ?room))
)


(defrule executed-plan-drop-same
	;(declare (salience -500))
	?f0 <- (executed plan ?num drop_same 1)
        ?f1 <- (plan (number ?num)(actions drop_same ?object))
        =>
        (retract ?f1)
        (retract ?f0)
        (printout t "delete drop_same plan " ?num " " ?object  crlf)
)

;(executed plan 72 find-object 1)
;(plan (name cubes) (number 72) (actions find-object book))
(defrule executed-plan-find-object
        ?f0 <- (executed plan ?num find-object 1)
        ?f1 <- (plan (number ?num)(actions find-object ?object))
        =>
        (retract ?f0 ?f1)
        ;(printout t "delete find-object plan " ?num " " ?object  crlf)
)



(defrule executed-plan-mv-object 
        ?f0 <- (executed plan ?num mv 1)
        ?f1 <- (plan (number ?num)(actions mv ?object))
        =>
        (retract ?f0 ?f1)
        ;(printout t "delete mv " ?num " " ?object  crlf)
)

(defrule executed-plan-grab-object  
        ?f0 <- (executed plan ?num grab 1)
        ?f1 <- (plan (number ?num)(actions grab ?object))
        =>
        (retract ?f0 ?f1)
        ;(printout t "delete grab " ?num " " ?object  crlf)
)

(defrule executed-plan-go-place
        ?f0 <- (executed plan ?num go 1)
        ?f1 <- (plan (number ?num)(actions go ?object))
        =>
        (retract ?f0 ?f1)
        ;(printout t "delete go " ?num " " ?object  crlf)
)

(defrule executed-plan-mv-object
        ?f0 <- (executed plan ?num mv 1)
        ?f1 <- (plan (number ?num)(actions mv ?object))
        =>
        (retract ?f0 ?f1)
        ;(printout t "delete mv " ?num " " ?object  crlf)
)



(defrule exe-plan-executed-goto
        ?f <-  (answer ?sender command goto ?room ?zone ?x ?y ?flg)
        ;?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions goto ?room ?zone))
        =>
        (retract ?f)
        (modify ?f2 (status accomplished))
)


; The robot should go to an specific position
(defrule exe-plan-go
        (plan (name ?name) (number ?num-pln)(status active)(actions go ?obj)(duration ?t))
        ?f1 <- (item (name ?obj) (zone ?zone)(pose ?x ?y ?z))
        =>
        (bind ?command (str-cat "" ?zone " " ?x " " ?y " " ?z""))
        (printout t "go " ?command  crlf)
        (assert (send-ROS ACT-PLN go ?command ?t 4))
)


(defrule exe-plan-executed-go
        ?f <-  (answer ?sender command go ?x ?y ?z ?flg)
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions go ?obj))
        =>
        (retract ?f)
        (modify ?f2 (status accomplished))
)

; The robot moves the specified angle and distance
(defrule exe-plan-mv
        (plan (name ?name) (number ?num-pln)(status active)(actions mv ?obj)(duration ?t))
        ?f1 <- (item (name ?obj) (zone ?zone)(pose ?x ?y ?z))
        =>
        ;(bind ?command (str-cat "" ?distance " " ?angle""))
        (bind ?command (str-cat "" ?x " " ?y""))
        (printout t "mv " ?command  crlf)
        (assert (send-ROS ACT-PLN mv ?command ?t 4))
)


(defrule exe-plan-executed-mv
        ?f <-  (answer ?sender command mv ?distance ?angle ?flg)
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions mv ?obj))
        =>
        (retract ?f)
        (modify ?f2 (status accomplished))
)




(defrule exe-plan-no-found-object
        ?f <-  (answer ?sender command find_object ?block1 ?x ?y ?z ?arm 0)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions find-object ?object))
        =>
        (retract ?f)
        (modify ?f2 (status active))
)


(defrule exe-plan-move-actuator
        (plan (number ?num-pln)(status active)(actions move ?arm ?obj)(duration ?t))
 	(item (name ?obj) (pose ?x ?y ?z) )
        (Arm (name ?arm))
        =>
        ;(bind ?command (str-cat "" ?obj " " ?x " " ?y " " ?z ""))
        (bind ?command (str-cat "" ?obj ""))
        ;(bind ?command (str-cat "" ?obj " nil"""))
        (assert (send-ROS ACT-PLN move_actuator ?command ?t 4))
)

(defrule exe-plan-moved-actuator
        ?f <-  (answer ?sender command move_actuator ?object 1)
        ;?f <-  (answer ?sender command move_actuator ?object ?x ?y ?z ?id 1)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions move ?actuator ?object))
	;?f3 <- (wait plan ?name ?num-pln ?t)
        =>
        (retract ?f)
        (modify ?f2 (status accomplished))
        ;(retract ?f3)
)

;fix this later
(defrule exe-plan-no-moved-actuator
        ?f <-  (answer ?sender command move_actuator ?object ?x ?y ?z ?id 0)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions move ?actuator ?object))
        ;?f3 <- (wait plan ?name ?num-pln ?t)
        =>
        (retract ?f)
        (modify ?f1 (name ?object))
)


(defrule exe-plan-grab-actuator
        (plan (name ?name) (number ?num-pln)(status active)(actions grab ?obj)(duration ?t))
        ?f1 <- (item (name ?obj))
        =>
        (bind ?command (str-cat "" ?obj ""))
        (assert (send-ROS ACT-PLN grab ?command ?t 4))
)


(defrule exe-plan-grabed-actuator
        ?f <-  (answer ?sender command grab ?object 1)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions grab ?object))
        ?f3 <- (Arm)
        =>
        (retract ?f)
	(modify ?f1 (status grabed)) 
        (modify ?f2 (status accomplished))
	(modify ?f3 (grasp ?object)) 
)


(defrule exe-plan-no-grabed-actuator
        ?f <-  (answer ?sender command grab ?actuator ?object 0)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions grab ?actuator ?object))
        ?f3 <- (item (name robot))
        ;?f4 <- (wait plan ?name ?num-pln ?t)
        =>
        (retract ?f)
        (modify ?f1 (name ?object))
)


(defrule exe-plan-drop-actuator
        (plan (name ?name) (number ?num-pln)(status active)(actions drop ?obj)(duration ?t))
        ?f1 <- (item (name ?obj))
        ?f2 <- (Arm (grasp ?obj))
        =>
        (bind ?command (str-cat "" ?obj ""))
        (assert (send-ROS ACT-PLN drop ?command ?t 4))
)

( defrule exe-plan-droped-actuator
        ?f <-  (answer ?sender command drop ?object ?x ?y ?theta 1)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions drop ?object))
        ?f3 <- (Arm (grasp ?object))
        =>
        (retract ?f)
	(modify ?f1 (status droped)(pose ?x ?y ?theta))
        (modify ?f2 (status accomplished))
        (modify ?f3 (status nil) (grasp nil))
)

(defrule exe-plan-no-droped-actuator
        ?f <-  (answer ?sender command drop ?actuator ?object ?flag 0)
        ?f1 <- (item (name ?object))
        ?f2 <- (plan (name ?name) (number ?num-pln)(status active)(actions drop ?actuator ?object))
        ?f3 <- (item (name robot))
        ;?f4 <- (wait plan ?name ?num-pln ?t)
        =>
        (retract ?f)
        (modify ?f1 (name ?object))
)



;(goal (move book) (room bedroom) (zone bed) (on apple))
;(goal-stack 3 bedroom bed book apple)
;(final-stack 3 bedroom bed apple)
(defrule update-stacks
	?f1 <- (stack ?room ?zone $?)
	?f2 <- (final-stack ?num ?room $?objs)
	?f3 <- (goal-stack ?num ?roon  $?objs)
	=>
	(retract ?f1 ?f2 ?f3)
	(assert (stack ?room ?zone $?objs))
)


(defrule delete-facts
	(declare (salience -10000))
	?f0 <- (finish-planner $?)
	?f1 <- (send-plan $?)
	=>
	(retract ?f0) 
	(retract ?f1) 
)


