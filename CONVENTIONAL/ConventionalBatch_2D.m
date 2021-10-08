clear all

Loop_vector_1 = [1:2:3]; % Set any range you like here
Loop_vector_2 = [4:8]; % Set any range you like here

for loop1=1:length(Loop_vector_1)
    % Set any quantity you'd like to vary here, eg
    for loop2=1:length(Loop_vector_1)
        for loop2=1:length(Cycle_vector)
            Paux = Aux_vector(loop1);
            Drive_cycle = Cycle_vector(loop2);
            InitiateEV;
            
            [loop1 loop2]
            close all
            sim('GeneralFEVmodel')
            if Drive_cycle==19,
                WbattPart(loop1,loop2,1:5)=Wbatt*[SOC(1,2) SOC(940,2) SOC(1720,2) SOC(2540,2) SOC(3500,2)]/100/3.6e6;
            elseif Drive_cycle==20
                WbattPart(loop1,loop2,1:5)=Wbatt*[SOC(1,2) SOC(860,2) SOC(1700,2) SOC(2540,2) SOC(3500,2)]/100/3.6e6;
            elseif Drive_cycle==21
                WbattPart(loop1,loop2,1:5)=Wbatt*[SOC(1,2) SOC(690,2) SOC(1330,2) SOC(1950,2) SOC(2720,2)]/100/3.6e6;
            else
                ['error']
            end
        end
    end
    
    
    for i=1:3
        WbattNoAC(i,1)= Wbatt/3.6e6;
        WbattNoAC(i,2:6)= WbattPart(1,i,1:5);
        WbattAC(i,1)= Wbatt/3.6e6;
        WbattAC(i,2:6)= WbattPart(2,i,1:5);
    end
    
    if loop3==1
        save WbattNoAC WbattNoAC
        save WbattAC WbattAC
        xlswrite('Mstad',WbattNoAC,'6tonNoAC','B4:G6')
        xlswrite('Mstad',WbattAC,'6tonAC','B4:G6')
    else
        save WbattNoAC WbattNoAC
        save WbattAC WbattAC
        xlswrite('Mstad',WbattNoAC,'19tonNoAC','B4:G6')
        xlswrite('Mstad',WbattAC,'19tonAC','B4:G6')
    end
end
