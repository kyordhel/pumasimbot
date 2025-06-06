
;************************************************
;*						*
;*	objects_deftemplates definitions	*
;*						*
;*			J.Savage, UNAM		*
;*			1/5/20			*
;*						*
;************************************************


(deftemplate item
        (field type
                 (type SYMBOL)
                 (default nil)
        )
        (field name
                 (type SYMBOL)
                 (default nil)
        )
        (multifield status
                 (type SYMBOL)
                 (default nil)
        )
        (multifield attributes
                 (type SYMBOL)
                 (default nil)
        )
        (multifield pose
                 (type NUMBER)
                 (default 0 0 0)
        )
        (field grasp
                 (type SYMBOL)
                 (default nil)
        )
	(field room
                (type SYMBOL)
                (default nil)
        )
        (field zone
                 (type SYMBOL)
                 (default nil)
        )
        (field possession
                 (type SYMBOL)
                 (default nobody)
        )
        (field image
                 (type SYMBOL)
                 (default nil)
        )
        (field script
                 (type SYMBOL)
                 (default nil)
        )
        (field num
                 (type NUMBER)
                 (default 1)
        )
        (field shared
                 (type SYMBOL)
                 (default false)
        )
	(multifield zones
                 (type SYMBOL)
                 (default nil)
        )
	(multifield hands
                 (type SYMBOL)
                 (default nil)
        )

)




(deftemplate Human
	(field name
		 (type SYMBOL)
		 (default nil)
	)
	(multifield objs
		 (type SYMBOL)
		 (default nil)
	)
	(field room
		(type SYMBOL)
                 (default livingroom)
        )
	(field zone
		 (type SYMBOL)
		 (default nil)
	)
	(field status
                 (type SYMBOL)
                 (default nil)
        )
	(multifield pose
                 (type NUMBER)
                 (default 0 0 0)
        )
	(field shared
                 (type SYMBOL)
                 (default false)
        )

	
)


(deftemplate Robot
	(field name
		 (type SYMBOL)
		 (default nil)
	)
	(field room
                (type SYMBOL)
                 (default nil)
        )
	(multifield hands
		 (type SYMBOL)
		 (default nil)
	)
	(multifield tray
		 (type SYMBOL)
		 (default nil)
	)
	(field manipulator
		 (type SYMBOL)
		 (default lower)
	)
	(field zone
		 (type SYMBOL)
		 (default nil)
	)
	(field status
                 (type SYMBOL)
                 (default nil)
        )
	(multifield pose
                 (type NUMBER)
                 (default 0 0 0)
        )
	(field shared
                 (type SYMBOL)
                 (default false)
        )

 )


(deftemplate Door
	(field name
		 (type SYMBOL)
		 (default nil)
	)
	(field status
                 (type SYMBOL)
                 (default nil)
        )
	(multifield attributes
		 (type SYMBOL)
		 (default nil)
	)
	(field grasp
		 (type SYMBOL)
		 (default nil)
	)
	(field zone
		 (type SYMBOL)
		 (default nil)
	)
	(field possession
		 (type SYMBOL)
		 (default nobody)
	)
	(field image
                 (type SYMBOL)
                 (default nil)
        )
	(field script
		 (type SYMBOL)
		 (default nil)
	)
	(field num
		 (type NUMBER)
		 (default 1)
	)
	(field shared
                 (type SYMBOL)
                 (default false)
        )

 )


(deftemplate Room
	(field name
		 (type SYMBOL)
		 (default nil)
	)
	(field status
                 (type SYMBOL)
                 (default nil)
        )
	(multifield attributes
		 (type SYMBOL)
		 (default nil)
	)
	(field grasp
		 (type SYMBOL)
		 (default nil)
	)
	(field zone
                 (type SYMBOL)
                 (default nil)
        )
	(multifield zones
		 (type SYMBOL)
		 (default nil)
	)
	(field possession
		 (type SYMBOL)
		 (default nobody)
	)
	(field image
                 (type SYMBOL)
                 (default nil)
        )
	(field script
		 (type SYMBOL)
		 (default nil)
	)
	(field num
		 (type NUMBER)
		 (default 1)
	)
	(field shared
                 (type SYMBOL)
                 (default false)
        )
	(multifield center
                 (type NUMBER)
                 (default 0 0)
        )
 )



(deftemplate Furniture
	(field name
		 (type SYMBOL)
		 (default nil)
	)
	(field status
		 (type SYMBOL)
		 (default nil)
	)
	(multifield attributes
		 (type SYMBOL)
		 (default nil)
	)
	(field grasp
		 (type SYMBOL)
		 (default nil)
	)
	(field zone
		 (type SYMBOL)
		 (default nil)
	)
	(field possession
		 (type SYMBOL)
		 (default nobody)
	)
	(field image
                 (type SYMBOL)
                 (default nil)
        )
	(field script
		 (type SYMBOL)
		 (default nil)
	)
	(field num
		 (type NUMBER)
		 (default 1)
	)
	(field shared
                 (type SYMBOL)
                 (default false)
        )

 )


(deftemplate Arm
        (field name
                 (type SYMBOL)
                 (default nil)
        )
        (field status
                 (type SYMBOL)
                 (default nil)
        )
        (multifield attributes
                 (type SYMBOL)
                 (default nil)
        )
        (field possession
                 (type SYMBOL)
                 (default nobody)
        )
        (field grasp
                 (type SYMBOL)
                 (default nil)
        )
        (field bandera
                 (type SYMBOL)
                 (default nil)
        )
)


(deftemplate plan
        (field name
                 (type SYMBOL)
                 (default nil)
        )
        (field number
                (type NUMBER)
                (default 1)
        )
	(multifield actions
		(type SYMBOL)
	)
        (field duration
                (type NUMBER)
                (default 1)
        )
        (field status
                 (type SYMBOL)
                 (default inactive)
        )
	(
	 field statusTwo
		(type SYMBOL)
		(default active)
	)

)





(deftemplate goal (slot move)(slot room)(slot zone)(slot on))
(deftemplate attempt (slot move)(slot room)(slot zone)(slot on)(slot number)(field status (type SYMBOL) (default nil)) )

