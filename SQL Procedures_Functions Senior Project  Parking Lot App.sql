/*
Runs at midnight with the query: 'IF(CONVERT(VARCHAR(8),GETDATE(),108) LIKE '00:00%')'. Neccessary to reset lots every night. 
Program automatically will run at midnight using Windows Scheduler. 
*/


--Updates lot01 sets available to 'Y', username = NULL, ramID NULL. 
exec updateLot01Proc

--Syntax of Proc

/****** Object:  StoredProcedure [dbo].[updateLot01Proc]    Script Date: 12/17/2019 4:19:49 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[updateLot01Proc]
AS
IF(CONVERT(VARCHAR(8),GETDATE(),108) LIKE '00:00%')
BEGIN
exec AdminReserveUpdate
update lot01 set Available = 'Y',RamID = 'NULL',UserName = 'NULL' where SpotNumber BETWEEN 1 AND 150;
        print 'Lot01 Updated'
		END
		ELSE
BEGIN
	print 'Lot01 Not Updated'
	END
	
	
--Updates lot02 sets available to 'Y', username = NULL, ramID NULL.
exec updateLot02Proc 

--Syntax of Proc

/****** Object:  StoredProcedure [dbo].[updateLot02Proc]    Script Date: 12/17/2019 4:19:57 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[updateLot02Proc]
AS
IF(CONVERT(VARCHAR(8),GETDATE(),108) LIKE '00:00%')
BEGIN
exec AdminReserveUpdate
update lot02 set Available = 'Y',RamID = 'NULL',UserName = 'NULL' where SpotNumber BETWEEN 1 AND 150;
        print 'Lot02 Updated'
		END
		ELSE
BEGIN
	print 'Lot02 Not Updated'
	END



 --Updates lot03 sets available to 'Y', username = NULL, ramID NULL.
exec updateLot03Proc

--Syntax of Proc

/****** Object:  StoredProcedure [dbo].[updateLot03Proc]    Script Date: 12/17/2019 4:20:05 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[updateLot03Proc]
AS
IF(CONVERT(VARCHAR(8),GETDATE(),108) LIKE '00:00%')
BEGIN
exec AdminReserveUpdate
update lot03 set Available = 'Y',RamID = 'NULL',UserName = 'NULL' where SpotNumber BETWEEN 1 AND 150;
        print 'Lot03 Updated'
		END
ELSE
BEGIN
	print 'Lot03 Not Updated'
	END



/*
Updates course to set start_time equal to the current time, in order to get around the CK_Lot01 check constraint. 
Mainly used as a nested proc.
*/
exec AdminReserveUpdate 

--Syntax of Proc

/****** Object:  StoredProcedure [dbo].[AdminReserveUpdate]    Script Date: 12/17/2019 4:18:09 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[AdminReserveUpdate]
AS
DECLARE @SpotNumber int
update course set start_time = (convert(VARCHAR, DATEADD(MINUTE, -300, GETDATE()), 100)) where CRN = '55555'
update Lot01 set Available = 'N', RamID = 'ADMIN' where SpotNumber = @SpotNumber





/*
Procedure used for reserving multiple spots if the date in dbo.PendingRequests is equal to the current date (GETDATE()). 
Will reserve the number of spots ranging from 1 to x depending on the number of spots reserved in dbo.PendingRequests
*/
exec MorningAdminProcedure s

--Syntax of Proc

/****** Object:  StoredProcedure [dbo].[MorningAdminProcedure]    Script Date: 12/17/2019 4:17:16 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[MorningAdminProcedure]
AS
BEGIN
IF EXISTS (select * from PendingRequests where dateOfEvent = (convert(VARCHAR, DATEADD(MINUTE, -300, GETDATE()), 100))) 
BEGIN
   exec AdminReserveUpdate
update lot01 set Available = 'N',RamID = 'ADMIN',UserName = 'ADMINTEST' where SpotNumber BETWEEN 1 AND (Select numSpots from PendingRequests where dateOfEvent = (convert(VARCHAR, DATEADD(MINUTE, -300, GETDATE()), 100)))        print 'Lot01 Updated'
END
END


--FUNCTION


exec dbo.Lot01CheckConstraintFunction @RamID = 'R01532354'

/*
Function mainly used for the Lot01 check constraint. 
If this function returns true, allow a user to reserve a spot in lot01. 
This function checks to see if a student has a class within 15 minutes before the class starts, for that specific @RamID
*/

--Syntax of Function:


/****** Object:  UserDefinedFunction [dbo].[Lot01CheckConstraintFunction]    Script Date: 12/17/2019 4:24:11 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER FUNCTION [dbo].[Lot01CheckConstraintFunction](
 @RamID char(5)
)
RETURNS VARCHAR(5)
AS
BEGIN
    IF EXISTS (select s.ramID
from dbo.schedule_test s join dbo.course c ON 
s.class1 = c.CRN 
or
s.class2 = c.CRN
or
s.class3 = c.CRN
or
s.class4 = c.CRN
or
s.class5 = c.CRN
or
s.class6 = c.CRN
where c.start_time BETWEEN (convert(VARCHAR, DATEADD(MINUTE, -300, GETDATE()), 100))
AND (convert(VARCHAR, DATEADD(MINUTE, -285, GETDATE()), 100)))
        return 'True'
    return 'False'
END
