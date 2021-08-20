; From Page 149 of "LISP" 1st edition.

; This code has been translated to Common Lisp
; from the dialect used in the book:
; table -> *table*
; (store (table n) x) -> (setf (aref table n) x)
; (boole 1 ...) -> (boole boole-and ...)
; greaterp -> >
; lessp -> <
; (quotient x y) -> (floor x y)
; add1 -> 1+
; sub1 -> 1-

(defparameter *table* #(
                        521
                        543
                        451
                        542
                        371
                        121
                        172
                        337
                        318
                        467
                        318
                        444
                        ))

(defun radix-sort (bottom top bit)
  (prog (bottom-n top-n temp-b temp-t)
        (cond ((zerop bit) (return nil)))
        (setq bottom-n bottom
              top-n top)
        up
        (cond ((> bottom-n top) (go simple)) 
              ((not (zerop (boole boole-and (aref *table* bottom-n) bit)))
               (go down))
              (t (setq bottom-n (1+ bottom-n))
                 (go up)))
        down
        (cond ((< top-n bottom)
               (go simple))
              ((zerop (boole boole-and (aref *table* top-n) bit))
               (go out))
              (t (setq top-n (1- top-n))
                 (go down)))
        out
        (cond ((< top-n bottom-n)
               (go finish))
              (t (setq temp-b (aref *table* bottom-n)
                       temp-t (aref *table* top-n))
                 (setf (aref *table* bottom-n) temp-t)
                 (setf (aref *table* top-n) temp-b)
                 (go up)))
        simple
        (radix-sort bottom top (floor bit 2))
        (return nil)
        finish
        (radix-sort bottom top-n (floor bit 2))
        (radix-sort bottom-n top (floor bit 2))))

(pprint "sorted")
(pprint *table*)
; BIT should be a power of two larger than half the largest number in the array
(radix-sort 0 (1- (length *table*)) 65536)

(pprint "unsorted")
(pprint *table*)

