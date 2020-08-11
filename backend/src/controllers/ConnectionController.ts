import {Request, Response} from "express";

export default class ConnectionController{
    async set(req:Request, res:Response){
        const rfid = req.query.rfid;

        if(rfid){
            console.log("New connection:",rfid);

            res.status(200).json({
                message: "No user found"
            });
            //CHECAR NO BANCO SE EXISTE ALGUÉM COM ESSE RFID, SE TIVER MARCAR PRESENÇA, SE NÃO EMITIR O SOCKET PARA OS CLIENTES
        }else{
            res.status(400).json({
                error: "Missing RFID value"
            });
        }
    }
}