
;************************************************
;*                                              *
;*      sockets_clips.clp                       *
;*                                              *
;*      Jesus Savage                            *
;*                                              *
;*              Bio-Robotics Laboratory         *
;*              UNAM, 2025                      *
;*                                              *
;*                                              *
;************************************************


(defrule clips-alive
        ?f <- (alive clips)
        =>
        (retract ?f)
	(broadcast "Clips alive")
        (printout t "Clips alive" crlf)
)



(defrule send-ros-num_plans
        (declare (salience 100))
        ?f <-  (step ?n)
        ?f1 <- (send-ROS ?system num_plans ?npl ?t ?num)
        =>
        (retract ?f ?f1)
	(bind ?message (str-cat ?system " num_plans " ?npl " " ?t " " ?num ))
        (broadcast ?message)

)



(defrule send-plan-two-arguments
	;(declare (salience 100))
        ?f <-  (step ?nm)
        ?f1 <- (send-ROS ?system ?nm goto ?arg1 ?arg2)
        =>
        (retract ?f ?f1)
        (bind ?message (str-cat ?system " plan " ?nm " goto "  ?arg1 " " ?arg2 ))
        (broadcast ?message)

)



(defrule send-plan-one-argument
        ;(declare (salience 100))
        ?f <-  (step ?nm)
        ?f1 <- (send-ROS ?system ?nm ?action ?argument)
        =>
        (retract ?f ?f1)
        (bind ?message (str-cat ?system " plan " ?nm " " ?action " "  ?argument))
        (broadcast ?message)
)




